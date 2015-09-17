#!/bin/sh
### BEGIN INIT INFO
# Provides:          svn_serve
# Required-Start:    $remote_fs
# Required-Stop:     $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start or stop the HTTP Proxy.
### END INIT INFO
case $1 in
    start)
        setsid /usr/local/bin/diag -l &
        ;;
    stop)
        killall diag
        ;;
*)
echo "Usage: $0 (start|stop)"
;;
esac