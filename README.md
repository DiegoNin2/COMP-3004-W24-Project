# COMP3004FinalProject
2024/4/17 Update logs:
1. Restructured the whole program so that now it follows the mediator pattern whereas the device acting like the mediator manages the control flow of all other classes
2. Several bug fix that caused the program to crash
3. Implemented the date and time option, whenever the user selects this option, there current date and time will be displayed on the text window above the current menu
4. Start, pause and stop button partially implemented, right now when they are pressed, a corresponding indication message will be displayed in the console, and a signal of start, pause or stop will be sent and eventually should be caught by the EEG progress handler and wave form handler in the mainwindow class to manage the treament work flow
5. Comments are added to most methods
