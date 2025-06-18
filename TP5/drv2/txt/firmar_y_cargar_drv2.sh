#!/usr/bin/env bash
#
# firmar_y_cargar_drv2.sh  –  Firma drv2.ko y lo carga mostrando el estado de taint.
# Ejecutar con: sudo ./firmar_y_cargar_drv2.sh
#
set -euo pipefail

KO_FILE="drv2.ko"
PRIV_KEY="MOK_drv2.priv"
PUB_KEY="MOK_drv2.pem"
COMMON_NAME="/CN=drv2_signing_key/"
DAYS_VALID=36500

echo "==> 1. Verificando que \$KO_FILE exista…"
if [[ ! -f \$KO_FILE ]]; then
  echo "ERROR: No se encontró \$KO_FILE en \$(pwd)." >&2
  exit 1
fi

echo "==> 2. Creando par de claves si es necesario…"
if [[ ! -f \$PRIV_KEY || ! -f \$PUB_KEY ]]; then
  openssl req -new -x509 -newkey rsa:2048 \
              -keyout "\$PRIV_KEY" -out "\$PUB_KEY" -nodes \
              -days "\$DAYS_VALID" -subj "\$COMMON_NAME"
  echo "   • Claves creadas: \$PRIV_KEY  /  \$PUB_KEY"
else
  echo "   • Claves ya existen, se reutilizarán."
fi

echo "==> 3. Firmando módulo con kmodsign…"
kmodsign sha256 "\$PRIV_KEY" "\$PUB_KEY" "\$KO_FILE"
echo "   • Módulo firmado correctamente."

echo "==> 4. Registrando clave con mokutil (puede requerir reinicio)…"
if mokutil --list-enrolled | grep -q "\$COMMON_NAME"; then
  echo "   • Clave ya matriculada; se omite."
else
  mokutil --import "\$PUB_KEY"
  echo "   • IMPORTADA. Matricula la clave en el próximo arranque."
  read -rp "Presiona ENTER para continuar…"
fi

echo "==> 5. Instalando módulo en /lib/modules y actualizando dependencias…"
install -m 644 "\$KO_FILE" "/lib/modules/\$(uname -r)/extra/"
depmod -a

echo "==> 6. Cargando módulo firmado…"
modprobe drv2

echo "==> 7. Línea en /proc/devices y flags de taint:"
grep SdeC_Driver2 /proc/devices || true
cat /sys/module/drv2/taint
echo "==> Listo."
