#!/usr/bin/env python3
"""
Convert Zephyr Kconfig .config file and inject PREDEFINED into a doxygen Doxyfile.

Reads a Kconfig .config file, generates PREDEFINED macro list,
and replaces the PREDEFINED line in the target Doxyfile.

Usage:
    python3 config_to_predefined.py <config_file> --doxyfile <doxyfile_path>

Conversion rules:
    CONFIG_FOO=y              ->  CONFIG_FOO
    CONFIG_FOO=123            ->  CONFIG_FOO=123
    # CONFIG_FOO is not set   ->  (skipped)
"""

import sys
import os
import argparse
import re


def parse_config(config_path):
    """Parse Kconfig .config file, yield (name, value) tuples."""
    with open(config_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            m = re.match(r'^(CONFIG_[A-Za-z0-9_]*)=(.*)$', line)
            if m:
                name = m.group(1)
                value = m.group(2)
                if value == 'y':
                    yield (name, None)
                else:
                    yield (name, value)


def generate_predefined_block(macros):
    """Generate PREDEFINED block (single line) for Doxyfile."""
    entries = []
    for name, value in macros:
        if value is None:
            entries.append(name)
        else:
            entries.append("{}={}".format(name, value))
    # Use spaces as separators; doxygen 1.9.8 has issues with backslash continuation
    return "PREDEFINED             = {}\n".format(' \\\n                         '.join(entries))


def inject_into_doxyfile(doxyfile_path, predefined_block):
    """Replace the PREDEFINED line in Doxyfile with the generated block."""
    with open(doxyfile_path, 'r') as f:
        content = f.read()

    # Match PREDEFINED = ... (with possible line continuations)
    pattern = r'PREDEFINED\s*=\s*(?:.*\\\n)*.*\n'
    match = re.search(pattern, content)
    if not match:
        print("Error: PREDEFINED not found in {}".format(doxyfile_path), file=sys.stderr)
        sys.exit(1)

    new_content = content[:match.start()] + predefined_block + content[match.end():]

    with open(doxyfile_path, 'w') as f:
        f.write(new_content)

    print("Updated PREDEFINED in {} ({} macros)".format(
        doxyfile_path, len(predefined_block.strip().split('\n')) - 1))


def main():
    parser = argparse.ArgumentParser(
        description='Convert Zephyr .config and inject PREDEFINED into Doxyfile')
    parser.add_argument('config', help='Path to Kconfig .config file')
    parser.add_argument('--doxyfile', required=True,
                        help='Path to Doxyfile to modify')
    args = parser.parse_args()

    macros = list(parse_config(args.config))
    if not macros:
        print("Warning: no CONFIG_* entries found in {}".format(args.config),
              file=sys.stderr)

    block = generate_predefined_block(macros)
    inject_into_doxyfile(args.doxyfile, block)


if __name__ == '__main__':
    main()
