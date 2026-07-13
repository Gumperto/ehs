#!/usr/bin/env bash

set -euo pipefail

# Gets the root directory
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# The data dir is by default ROOT_DIR, but you may change it to something else
DATA_DIR="$ROOT_DIR"

EHS_BINARY_PATH="$ROOT_DIR/build/ehs"
if [ ! -f "$EHS_BINARY_PATH" ]; then
    echo -e "WARNING: $EHS_BINARY_PATH does not exist yet! Build the project using 'make'\n"
fi

cat > "$ROOT_DIR/.env" << EOF
ROOT_DIR=$ROOT_DIR
DATA_DIR=$DATA_DIR
EHS_BINARY_PATH=$EHS_BINARY_PATH
EOF

echo "Wrote .env to $ROOT_DIR/.env"
cat "$ROOT_DIR/.env"
