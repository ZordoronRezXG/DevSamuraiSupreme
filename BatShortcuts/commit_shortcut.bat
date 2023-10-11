@echo off

set /p commit_message=<commit_msg.txt
cd ..


git add .
git commit -m "%commit_message%"
git push
