/****************************************************************************
 *
 * Copyright (c) 2023 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/
/**
 * @file    console.c
 * @brief   FreeRTOS Console module.
 *
 * @addtogroup OS
 * @addtogroup UTILITIES
 * @ingroup OS
 * @addtogroup CONSOLE
 * @ingroup UTILITIES
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <FreeRTOS.h>
#include <FreeRTOS_CLI.h>
#include <task.h>

#include <typedefs.h>
#include <console.h>

/*===========================================================================*/
/* External objects                                                          */
/*===========================================================================*/

/*===========================================================================*/
/* Global variables                                                          */
/*===========================================================================*/

/*===========================================================================*/
/* Constants and macros                                                      */
/*===========================================================================*/

#define CONSOLE_VERSION   "1.0.0"

#define stringify(_x_)    #_x_
#define BOARDNAME(_a_)    stringify(_a_)

#define CONSOLE_MAX_COMMAND_INPUT_LENGTH    configCOMMAND_INT_MAX_INPUT_SIZE
#define CONSOLE_MAX_COMMAND_OUTPUT_LENGTH   configCOMMAND_INT_MAX_OUTPUT_SIZE
#define CONSOLE_DISABLE_PROMPT              configCLI_DISABLE_CONSOLE_PROMPT

#define CONSOLE_TASK_STACK_SIZE             configCONSOLE_STACK_SIZE
#define CONSOLE_TASK_PRIORITY               configCONSOLE_TASK_PRIORITY

static const char * const welcome =
    "\r\nConsole command server.\r\nType Help to view a list of registered commands.\r\n";

static const char * const prompt = BOARDNAME(BOARD_NAME)"> ";

/* Console history */
#define CONSOLE_HISTORY_BUFFER_LENGTH    16U

/*===========================================================================*/
/* Local types                                                               */
/*===========================================================================*/

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Local variables                                                           */
/*===========================================================================*/

/* Input command buffer */
static char inputString[CONSOLE_MAX_COMMAND_INPUT_LENGTH];

/* Console Task handle. */
static TaskHandle_t consoleTaskHandle;

/* Console Task Control Block (TCB). */
static StaticTask_t consoleTaskTCB;

/* Console Task stack (4 Kb). */
static StackType_t consoleTaskStack[CONSOLE_TASK_STACK_SIZE];

/* Console prompt enabled by default. */
#if (CONSOLE_DISABLE_PROMPT == TRUE)
static BaseType_t consolePromptDisable = pdTRUE;
#else
static BaseType_t consolePromptDisable = pdFALSE;
#endif

static char historyCmdBuf[CONSOLE_HISTORY_BUFFER_LENGTH][CONSOLE_MAX_COMMAND_INPUT_LENGTH];

/* Next available free slot (can override old commands */
static uint32_t historyNext;

/* First valid command idx */
static uint32_t historyFirst;

/* Last valid command idx */
static uint32_t historyLast;

/*===========================================================================*/
/* Local functions.                                                          */
/*===========================================================================*/

static void consoleVersion(void){
    printf("Console version " CONSOLE_VERSION "\r\n");
}

static void consolePromptOff(void){
    consolePromptDisable = pdTRUE;
}

static void consolePromptOn(void){
    consolePromptDisable = pdFALSE;
}

static void historyInit(void) {
    historyFirst = 0;
    historyLast = 0;
    historyNext = 0;

    /* Clear history buffer */
    memset(historyCmdBuf, (int)'\0', (CONSOLE_HISTORY_BUFFER_LENGTH * CONSOLE_MAX_COMMAND_INPUT_LENGTH));
}

static void historyDisplay(void) {
    uint32_t idx;

    if (historyFirst != historyNext) {

        /* first available command */
        idx  = historyFirst;

        /* last available command is just before next */
        do {
            if (strlen(&historyCmdBuf[idx][0]) > 0U) {
                printf("% 3d:  %s\r\n", (int)idx, &historyCmdBuf[idx][0]);
            }

            idx++;
            if (idx == CONSOLE_HISTORY_BUFFER_LENGTH) {
                idx = 0;
            }
        } while (idx != historyFirst);
    }
}

static void historyAdd(const char * cmd) {
    char *slot;

    slot = &historyCmdBuf[historyNext][0];

    memcpy(slot, cmd, CONSOLE_MAX_COMMAND_INPUT_LENGTH);

    historyLast = historyNext;

    historyNext++;
    if (historyNext == CONSOLE_HISTORY_BUFFER_LENGTH) {
        historyNext = 0;
    }

    if (historyNext == historyFirst) {
        historyFirst++;
        if (historyFirst == CONSOLE_HISTORY_BUFFER_LENGTH) {
            historyFirst = 0;
        }
    }
}

static char const * historyGetBackward(void) {
    char *cmd;

    cmd = NULL;

    if (historyFirst != historyNext) {

        cmd = &historyCmdBuf[historyLast][0];

        if (historyLast == historyFirst) {
            if (historyNext == 0U) {
                historyLast = CONSOLE_HISTORY_BUFFER_LENGTH - 1U;
            } else {
                historyLast = historyNext - 1U;
            }
        } else {
            if (historyLast == 0U) {
                historyLast = CONSOLE_HISTORY_BUFFER_LENGTH - 1U;
            } else {
                historyLast--;
            }
        }
    }

    return cmd;
}

static char const * historyGetForward(void) {
    char *cmd;

    cmd  = NULL;

    if (historyFirst != historyNext) {

        cmd = &historyCmdBuf[historyLast][0];

        historyLast++;
    
        if (historyLast == CONSOLE_HISTORY_BUFFER_LENGTH) {
            historyLast = 0;
        }

        if (historyLast == historyNext) {
            historyLast = historyFirst;
        }
    }

    return cmd;
}

static void builtinHelp(void) {
    printf("Console built-in commands:\r\n");
    printf("  history:      console command history\r\n");
    printf("  clear:        clears console command history\r\n");
    printf("  version:      console version string\r\n");
    printf("  promptoff:    console prompt disable\r\n");
    printf("  prompton:     console prompt enable\r\n");
    printf("  !<num>:       executes command at <num> position in console history\r\n");
    printf("  <key up>:     display history commands in backward order\r\n");
    printf("  <key left>:   display history commands in backward order\r\n");
    printf("  <key down>:   display history commands in forward order\r\n");
    printf("  <key right>:  display history commands in forward order\r\n");
    printf("  builtin:      this help\r\n");
}

/* Manage built-in command */
static BaseType_t isBuiltInCommand(char *cmd) {

    BaseType_t ret = pdFALSE;

    if (strcmp(cmd, "builtin") == 0) {
        builtinHelp();
        ret = pdTRUE;

    } else if (strcmp(cmd, "version") == 0) {
        consoleVersion();
        ret = pdTRUE;

    } else if (strcmp(cmd, "promptoff") == 0) {
        consolePromptOff();
        ret = pdTRUE;

    } else if (strcmp(cmd, "prompton") == 0) {
        consolePromptOn();
        ret = pdTRUE;

    } else if (strcmp(cmd, "history") == 0) {
        historyDisplay();
        ret = pdTRUE;

    } else if (strcmp(cmd, "clear") == 0) {
        historyInit();
        ret = pdTRUE;

    } else if (cmd[0] == '!') {
        /* Command is in the form '!<number>'
         * Substitute the input command with the command
         * at position <number> from the history buffer */
        if (historyFirst == historyNext) {
            /* History buffer is empty */
        } else {
            uint32_t slot = (uint32_t)atoi(&cmd[1]);

            if (historyFirst < historyNext) {
                if (slot <= historyNext) {
                    memcpy(cmd, &historyCmdBuf[slot][0], CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                    printf("\r%s%s\r\n", prompt, cmd);
                }
            } else {
                memcpy(cmd, &historyCmdBuf[slot][0], CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                printf("\r%s%s\r\n", prompt, cmd);
            }
        }
    } else {
      /* required by MISRA */
    }

    return ret;
}

/*
 * Utility functions
 */

/* left trim space(s) from a string */
static char * left_trim(char *string) {
    while (*string == ' ') {
        string++;
    }

    return string;
}

/* right trim space(s) from a string */
static char *right_trim(char *string) {

    char *last = string + strlen(string) - 1;

    if (strlen(string) > 0U) {
        last = string + strlen(string) - 1;
        while (*last == ' ') {
            *last-- = '\0';
        }
    }

    return string;
}

/* right and left trim space(s) from a string */
static char *trim(char *string) {
    return right_trim(left_trim(string));
}

/**
 * @brief     Console task
 *
 * @api
 * @par Description
 * @details     Console parameter specifies the CLI command to execute before
 *              the prompt. If NULL, no command is executed.
 * @param       pvParameters console parameter.
 */
static void consoleTask(void *pvParameters) {

    int ch;
    uint8_t idx = 0U;
    BaseType_t moreData;
    char *outputString;
    char const * cmd;

    /* Initialize history */
    historyInit();

    /* Get output buffer from CLI */
    outputString = FreeRTOS_CLIGetOutputBuffer();

    /* The console parameter can specify a command that will be executed before
       the prompt.*/
    if (pvParameters != NULL) {
        do {
            /* Clear the output buffer */
            memset(outputString, (int)'\0', CONSOLE_MAX_COMMAND_OUTPUT_LENGTH);

            /* Send command to the interpreter */
            moreData = FreeRTOS_CLIProcessCommand((const char * const)pvParameters, outputString, CONSOLE_MAX_COMMAND_OUTPUT_LENGTH);

            /* Send the output of the command to the console */
            printf("%s", outputString);

        } while (moreData != pdFALSE);
    }

    if (consolePromptDisable == pdFALSE) {
        /* Send a welcome message to the user knows they are connected. */
        printf("%s", welcome);
        printf("\r\n%s", prompt);
    }

    for( ;; ) {

        ch = getchar();

        if ((char)ch == '\r') {
            if (consolePromptDisable == pdFALSE) {
                /* Newline */
                printf("\r\n");
            }

            if (idx > 0U) {
                /* trim spaces */
                cmd = trim(inputString);
                strcpy(inputString, cmd);

                if (strlen(inputString) > 0U) {
                    if (isBuiltInCommand(inputString) == pdFALSE) {

                        /* Add command to history */
                        historyAdd(inputString);

                        do {
                            /* Clear the output buffer */
                            memset(outputString, (int)'\0', CONSOLE_MAX_COMMAND_OUTPUT_LENGTH);

                            /* Send command to the interpreter */
                            moreData = FreeRTOS_CLIProcessCommand((const char * const)inputString, outputString, CONSOLE_MAX_COMMAND_OUTPUT_LENGTH);

                            /* Send the output of the command to the console */
                            printf("%s", outputString);

                        } while (moreData != pdFALSE);
                    }
                }

                idx = 0;
                memset(inputString, (int)'\0', CONSOLE_MAX_COMMAND_INPUT_LENGTH);
            }
            if (consolePromptDisable == pdFALSE) {
                printf("%s", prompt);
            }
        } else {
            if ((char)ch == '\n') {
                /* Ignore carriage returns. */
            } else if ((char)ch == '\b') {
                uint32_t i;

                /* Backspace: erase last character */
                if (idx > 0U) {
                    if (consolePromptDisable == pdFALSE) {
                        (void)putchar((int)'\r');
                        for (i = 0; i < strlen(prompt) + idx; i++) {
                            (void)putchar((int)' ');
                        }
                    }
                    inputString[--idx] = '\0';
                    if (consolePromptDisable == pdFALSE) {
                        printf("\r%s%s", prompt, inputString);
                    }
                }
            } else if ((uint8_t)ch == 0x1BU) {
                /* ignore escapes */
                /* skip the '[' */
                (void)getchar();

                /* This is the key pressed */
                ch = getchar();
                switch((char)ch) {
                    case 'A':
                        /* key up */
                    case 'D':
                        /* key left */
                        cmd = historyGetBackward();
                        if (cmd != NULL) {
                            /* clear the input line on console */
                            memset(inputString, (int)' ', CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                            inputString[CONSOLE_MAX_COMMAND_INPUT_LENGTH - 1U] = '\0';
                            if (consolePromptDisable == pdFALSE) {
                                printf("\r%s%s", prompt, inputString);
                            }

                            memcpy(inputString, cmd, CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                            idx = (uint8_t)strlen(inputString);
                            if (consolePromptDisable == pdFALSE) {
                                printf("\r%s%s", prompt, inputString);
                            }
                        }
                        break;
                    case 'B':
                        /* key down */
                    case 'C':
                        /* key right */
                        cmd = historyGetForward();
                        if (cmd != NULL) {
                            /* clear the input line on console */
                            memset(inputString, (int)' ', CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                            inputString[CONSOLE_MAX_COMMAND_INPUT_LENGTH - 1U] = '\0';
                            if (consolePromptDisable == pdFALSE) {
                                printf("\r%s%s", prompt, inputString);
                            }

                            memcpy(inputString, cmd, CONSOLE_MAX_COMMAND_INPUT_LENGTH);
                            idx = (uint8_t)strlen(inputString);
                            if (consolePromptDisable == pdFALSE) {
                                printf("\r%s%s", prompt, inputString);
                            }
                        }
                        break;
                    default:
                        /* required by MISRA */
                        break;
                }
            } else {
                /* A character was entered */
                if (idx < CONSOLE_MAX_COMMAND_INPUT_LENGTH) {
                    inputString[idx++] = (char)ch;
                    if (consolePromptDisable == pdFALSE) {
                        (void)putchar(ch);
                    }
                }
            }
        }
    }
}

/*===========================================================================*/
/* Global functions.                                                         */
/*===========================================================================*/

/**
 * @brief     Start console
 *
 * @api
 * @par Description
 * @details     Console parameter specifies the CLI command to execute before
 *              the prompt. If NULL, no command is executed.
 * @param       pvParameters console parameter.
 */
void startConsole( void *pvParameters )
{
    /* Start the console task */
    consoleTaskHandle = xTaskCreateStatic(consoleTask,
                                          (const char * const)"consoleTask",
                                          CONSOLE_TASK_STACK_SIZE,  /* Stack size in word */
                                          pvParameters,
                                          CONSOLE_TASK_PRIORITY,
                                          consoleTaskStack,
                                          &consoleTaskTCB);
}

/**
 * @brief     Stop console
 *
 * @api
 * @par Description
 * @details
 */
void stopConsole( void )
{
    /* Stop the console task */
    vTaskDelete( consoleTaskHandle );
}

/** @} */
