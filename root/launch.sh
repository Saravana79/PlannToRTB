#!/bin/bash

tmux kill-session -t rtb
tmux new-session -d -s rtb './build/x86_64/bin/launcher --node localhost --script ./launch.sh --launch sample.launch.json'
tmux rename-window 'launcher'
tmux new-window -d -t rtb:1 -n 'ad-server' 'tail -F ./logs/ad-server.log'
tmux new-window -d -t rtb:2 -n 'monitor' 'tail -F ./logs/monitor.log'
tmux new-window -d -t rtb:3 -n 'logger' 'tail -F ./logs/logger.log'
tmux new-window -d -t rtb:4 -n 'agent-configuration' 'tail -F ./logs/agent-configuration.log'
tmux new-window -d -t rtb:5 -n 'banker' 'tail -F ./logs/banker.log'
tmux new-window -d -t rtb:6 -n 'augmentor' 'tail -F ./logs/augmentor.log'
tmux new-window -d -t rtb:7 -n 'router' 'tail -F ./logs/router.log'
tmux new-window -d -t rtb:8 -n 'post-auction' 'tail -F ./logs/post-auction.log'
tmux new-window -d -t rtb:9 -n 'HelloRTBKit-agent-1' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:10 -n 'HelloRTBKit-agent-3' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:11 -n 'HelloRTBKit-agent-5' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:12 -n 'HelloRTBKit-agent-1-S' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:13 -n 'HelloRTBKit-agent-3-S' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:14 -n 'HelloRTBKit-agent-10' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:15 -n 'HelloRTBKit-agent-11' 'tail -F ./logs/HelloRTBKit-agent.log'
tmux new-window -d -t rtb:16 -n 'HelloRTBKit-agent-12' 'tail -F ./logs/HelloRTBKit-agent.log'
if [[ "$*" != *--quiet* ]]
then
    tmux attach -t rtb
fi