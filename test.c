/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"
#include <stdio.h>
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                       3        /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_TCB        *ptcb;
int             i;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void  Task1(void *data);
        void  Task2(void *data);                      /* Function prototypes of tasks                  */
        void  Task3(void *data);
        void  Print(void);
        void  ArgumentSet(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void  main (void)
{
    // PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

//    OSInit();                                              /* Initialize uC/OS-II                      */

   // PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
   // PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    OSTaskCreate(Task1, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Task2, (void *)0, &TaskStk[1][TASK_STK_SIZE - 1], 2);
//    OSTaskCreate(Task3, (void *)0, &TaskStk[2][TASK_STK_SIZE - 1], 3);
    ArgumentSet(); // to create buf(jasper)
    printf("in main\n");
    OSStart();                                             /* Start multitasking                       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void  Task1 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

    INT16S     key;
    int start;      /*the start time    */
    int end;        /*the end time      */
    int arrive;     /*  arrive time   */
    int toDelay;
    pdata = pdata;
    arrive = 0;

//    OS_ENTER_CRITICAL();
////    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
////    PC_SetTickRate(1u);                      /* Reprogram tick rate                      */
//    OS_EXIT_CRITICAL();

    while(1){
//        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
//            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
//                PC_DOSReturn();                            /* Return to DOS                            */
//            }
//        }
//    	printf("in task1\n");
        OS_ENTER_CRITICAL();
        Print();
        OS_EXIT_CRITICAL();
//        printf("int task1 exit critical\n");
        start = OSTimeGet();
        while(OSTCBCur->compTime>0){
//        	printf("in task loop\n");
        }

        OS_ENTER_CRITICAL();
        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-arrive);
        arrive += OSTCBCur->period;

        OSTCBCur->compTime = 1;

        if(toDelay<0)
            printf("Task1 deadline\n");
        else
            OSTimeDly(toDelay);
        OS_EXIT_CRITICAL();
    }
}
void  Task2 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

    int start;      /*  start time      */
    int end;        /*  end time        */
    int arrive;     /*  arrive time     */
    int toDelay;
    pdata = pdata;
    arrive = 0;
    while(1){
        // Print();

        start = OSTimeGet();
        while(OSTCBCur->compTime>0){}// keep looping if is not completed (jasper)

        // changing the value (jasper)
        OS_ENTER_CRITICAL();
        end = OSTimeGet(); // record the ending time(jasper)
        toDelay = (OSTCBCur->period) - (end-arrive);// calculate how much time left(period - computation time) (jasper)
        arrive += OSTCBCur->period;

        OSTCBCur->compTime = 3;

        if(toDelay<0)
            printf("Task2 deadline\n");
        else
            OSTimeDly(toDelay);
        OS_EXIT_CRITICAL();
    }
}
void  Task3 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    int start;      /*  start time      */
    int end;        /*  end time        */
    int arrive;     /*  arrive time     */
    int toDelay;
    int deadline = OSTCBCur->period;
    pdata = pdata;
    arrive = 0;
    while(1){
        // Print();

        start = OSTimeGet();
        while(OSTCBCur->compTime>0){}

        OS_ENTER_CRITICAL();
        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-arrive);
        arrive += OSTCBCur->period;

        OSTCBCur->compTime = 3;

        if(toDelay<0)
            printf("%d  Task3 deadline\n", deadline);
        else
            OSTimeDly(toDelay);
        deadline += OSTCBCur->period;
        OS_EXIT_CRITICAL();

    }
}
void Print(void){
    if(print_pos<pos){
        for(i=print_pos; i<pos; i++){
            printf("%d\t", buf[i][0]);
            if(buf[i][1]==0)    printf("Preempt\t");
            else                printf("Complete\t");
            printf("%d\t%d\n", buf[i][2], buf[i][3]);
        }
    }
    else{
        for(i=print_pos; i<row_size; i++){
            printf("%d\t", buf[i][0]);
            if(buf[i][1]==0)    printf("Preempt\t");
            else                printf("Complete\t");
            printf("%d\t%d\n", buf[i][2], buf[i][3]);
        }
        for(i=0; i<pos; i++){
            printf("%d\t", buf[i][0]);
            if(buf[i][1]==0)    printf("Preempt\t");
            else                printf("Complete\t");
            printf("%d\t%d\n", buf[i][2], buf[i][3]);
        }
    }
    print_pos = pos;
}
void ArgumentSet(void){
    ptcb = OSTCBList;
        //ptcb is a pointer which point to TCB (jasper)
        // OSTCBList is pointer of the TCBlist in used.

    // iterate through the tcblist(jasper)
    while(ptcb->OSTCBPrio==1 || ptcb->OSTCBPrio==2 || ptcb->OSTCBPrio==3){
        // printf("Priority: %d set argument\n", ptcb->OSTCBPrio);
        if(ptcb->OSTCBPrio==1){
            ptcb->compTime = 1;
            ptcb->period = 3;
        }
        else if(ptcb->OSTCBPrio==2){
            ptcb->compTime = 3;
            ptcb->period = 6;
        }
        else if(ptcb->OSTCBPrio==3){
            ptcb->compTime = 4;
            ptcb->period = 9;
        }
        ptcb = ptcb->OSTCBNext;
    }

    row_size = 5; // the total buffer row(jasper)
    col_size = 4; // the total buffer col for each row to show.(i.e. time_tick, behavior, from_task,to_task)(jasper)
    pos = 0;        // the saving row position(jasper)
    print_pos = 0;  // the print row position(jasper)
    buf = (int**)malloc(sizeof(int*)*row_size);// buffer to keep the info to print(jasper)
    for(i=0; i<row_size; i++)
        buf[i] = (int*)malloc(sizeof(int)*col_size);
}
