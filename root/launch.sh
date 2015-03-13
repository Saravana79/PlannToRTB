#!/bin/bash

tmux kill-session -t rtb
tmux new-session -d -s rtb './build/x86_64/bin/launcher --node localhost --script ./launch.sh --launch sample.launch.json'
tmux rename-window 'launcher'
tmux new-window -d -t rtb:1 -n 'ad-server' 'tail -f ./logs/ad-server.log'
tmux new-window -d -t rtb:2 -n 'monitor' 'tail -f ./logs/monitor.log'
tmux new-window -d -t rtb:3 -n 'logger' 'tail -f ./logs/logger.log'
tmux new-window -d -t rtb:4 -n 'agent-configuration' 'tail -f ./logs/agent-configuration.log'
tmux new-window -d -t rtb:5 -n 'banker' 'tail -f ./logs/banker.log'
tmux new-window -d -t rtb:6 -n 'augmentor' 'tail -f ./logs/augmentor.log'
tmux new-window -d -t rtb:7 -n 'router' 'tail -f ./logs/router.log'
tmux new-window -d -t rtb:8 -n 'post-auction' 'tail -f ./logs/post-auction.log'
tmux new-window -d -t rtb:9 -n 'HelloRTBKit-agent-1' 'tail -f ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:11 -n 'HelloRTBKit-agent-1-S' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
tmux new-window -d -t rtb:12 -n 'HelloRTBKit-agent-1-B' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
tmux new-window -d -t rtb:13 -n 'HelloRTBKit-agent-1-S-B' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
tmux new-window -d -t rtb:14 -n 'HelloRTBKit-agent-1-R' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
tmux new-window -d -t rtb:15 -n 'HelloRTBKit-agent-1-S-R' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
tmux new-window -d -t rtb:15 -n 'HelloRTBKit-agent-1-V' 'tail -f ./logs/HelloRTBKit-agent-1-S.log'
if [[ "$*" != *--quiet* ]]
then
    tmux attach -t rtb
fi