//******************************* FileTransferTasks ***************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferTasks.h
// Summary  : Header for File Transfer Manager RTOS task
// Author   : Anoop G
// Date     : 22-07-2025
//
//*****************************************************************************
#ifndef FILE_TRANSFER_TASKS_H
#define FILE_TRANSFER_TASKS_H

//***************************** Global Constants ******************************
#define FILE_TRANSFER_MNGR_TASK_STACK_SIZE		5120
#define FILE_TRANSFER_MNGR_TASK_NAME		    "FileTransferManagerTask"

//**************************** Forward Declarations ***************************
void FileTransferManagerTask(void *pvArgs);

#endif // FILE_TRANSFER_TASKS_H

//EOF
