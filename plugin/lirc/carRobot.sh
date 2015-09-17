#!/bin/sh
### BEGIN INIT INFO
# Provides:          carRobot
# Required-Start:    $remote_fs
# Required-Stop:     $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start or stop the HTTP Proxy. sudo update-rc.d svn_serve defaults
### END INIT INFO

LUA_CPATH="/usr/local/lib/lua/?.lua;;"

LUA_CPATH="/usr/local/lib/lua/5.1/?.so;;"

case $1 in
    start)
        setsid /usr/local/bin/carRobot -l &
        ;;
    stop)
        killall carRobot
        ;;
*)
echo "Usage: $0 (start|stop)"
;;
esac