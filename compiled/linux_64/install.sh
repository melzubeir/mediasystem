#!/bin/bash

cp socialhose.png /usr/share/icons/hicolor/32x32/apps
cp "Clipping Station.desktop" ~/Desktop/
cp "Clipping Station" /usr/local/bin/
cp socialhose.ini /etc/
apt-get -y install libqt4-sql-mysql
mkdir /mnt/filestow
cat fstab.txt >> /etc/fstab
mount /mnt/filestow
ln -s /mnt/filestow/clippings /clippings
ln -s /mnt/filestow/Issues /Issues
