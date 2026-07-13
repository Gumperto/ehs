#!/usr/bin/env bash

set -euo pipefail

# Gets the root directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

EHS_BINARY_PATH="$SCRIPT_DIR/build/ehs"

cat > "$SCRIPT_DIR/.env" << EOF
EHS_BINARY_PATH=$EHS_BINARY_PATH
EOF

echo "Wrote .env to $SCRIPT_DIR/.env"
cat "$SCRIPT_DIR/.env"
