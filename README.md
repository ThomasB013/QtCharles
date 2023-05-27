
This is my first Qt project. 

Idea is inspired from a first year course and a project setup from Ciske Harsema's using wxWidgets, but I did not use his code.
This project is used to teach first year CS students control structures with a UI.

# Student code
Commands.h: commands available for students.
Agent.h: student programs (that they can execute).

# Debug Trace
All actions are traced in the debug trace.
Debug trace gets actions that should be executed and appends these actions in the list and executes them on world.
Afterwards students can click/scroll in the listwidget and inspect execution step by step.

# World
Square world. Emits signals on changes / loads. 
Can read from and save to .txt files.
New worlds can be created (newworldialog).
Worldwidget: UI representation of world. Reacts to signal from world for updates.

# Mainwindow:
Contains a single world and a single debugtrace.
UI actions for files and robot actions.
