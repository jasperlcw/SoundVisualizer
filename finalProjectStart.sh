#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "Usage:"
    echo "./finalProjectStart.sh 0 -> This runs without elevated privilages"
    echo "./finalProjectStart.sh 1 -> This runs with elevated privilages. Potentially increases LED Panel performance."
    exit 1
fi

wget -q --spider --dns-timeout=2 http://google.com
if [[ $? -ne 0 ]]; then
    echo "Internet access needed to run the program for clock synchronization."
    exit 1
fi

for i in {35..46}
do
    config-pin p8.$i gpio > /dev/null
done

if [[ $1 -eq 0 ]]; then
    cd /mnt/remote/myApps && ./final_project
elif [[ $1 -eq 1 ]]; then
    cd /mnt/remote/myApps && sudo ./final_project
else
    echo "Usage:"
    echo "./finalProjectStart.sh 0 -> Runs visualizer without elevated privilages."
    echo "./finalProjectStart.sh 1 -> Runs visualizer with elevated privilages. Increases LED performance at the cost of data processing power."
    exit 1
fi

