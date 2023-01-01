#!/bin/bash

# Controllo se non c'è la cartella temporanea
if [ ! -d "/var/tmp/2048_ax3lt" ]
then
    mkdir -p /var/tmp/2048_ax3lt
    curl -s https://dm0qx8t0i9gc9.cloudfront.net/previews/audio/BsTwCwBHBjzwub4i4/button-radio-click-multiple_GkB5hYNd_NWM.mp3 -o /var/tmp/2048_ax3lt/move.mp3 > /dev/null
else
  if [ ! -e /var/tmp/2048_ax3lt/move.mp3 ]
  then
    curl -s https://dm0qx8t0i9gc9.cloudfront.net/previews/audio/BsTwCwBHBjzwub4i4/button-radio-click-multiple_GkB5hYNd_NWM.mp3 -o /var/tmp/2048_ax3lt/move.mp3 > /dev/null
  fi
fi


