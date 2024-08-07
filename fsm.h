/********************************************************************************
 * @file           : fsm.h
 * @author         : Christian Mahlburg
 * @date           : 31.07.2024
 * @brief          : Create simple table based finite state machines (FSM)
 *
 ********************************************************************************
 * MIT License
 *
 * Copyright (c) 2024 CMA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * The module can be used to create simple table based FSM instances.
 * Every FSM can hold up to FSM_MAX_NR_OF_STATES (1 - 255) with optional
 * entry and exit functions.
 *
 * No dynamic memory allocation is used. Be aware that the
 * fsm table always holds FSM_MAX_NR_OF_STATES items,
 * even if there are less states used for some FSM instances.
 *
 * Every time fsmRun(&fsmHandle) is called, the FSM is executed once.
 * On first call, initial state will be executed without entry function.
 *
 * Example usage:
 *
 * typedef enum
 * {
 *     FSM_STATE_INIT,
 *     FSM_STATE_INACTIVE,
 *     FSM_STATE_ACTIVE,
 * } FsmState_t;
 *
 * static uint8_t StateInit(void)
 * {
 *     uint8_t nextState = FSM_STATE_INACTIVE;
 *
 *     gpioInit(LED1_PORT, LED1_PIN, GPIO_OUPUT, GPIO_PIN_RESET);
 *     gpioInit(BUTTON1_PORT, BUTTON1_PIN, GPIO_INPUT);
 *
 *     return nextState;
 * }
 *
 * static void OnEntryInactive(void)
 * {
 *     gpioSet(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
 *     printf("Enter state INACTIVE\r\n");
 * }
 *
 * static uint8_t StateInactive(void)
 * {
 *     uint8_t nextState = FSM_STATE_INACTIVE;
 *
 *     if (GPIO_PIN_SET = gpioRead(BUTTON1_PORT, BUTTON1_PIN))
 *     {
 *         nextState = FSM_STATE_ACTIVE;
 *     }
 *
 *     return nextState;
 * }
 *
 * static void OnEntryActive(void)
 * {
 *     gpioSet(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
 *     printf("Enter state ACTIVE\r\n");
 * }
 *
 * static uint8_t StateActive(void)
 * {
 *     uint8_t nextState = FSM_STATE_ACTIVE;
 *
 *     if (GPIO_PIN_RESET = gpioRead(BUTTON1_PORT, BUTTON1_PIN))
 *     {
 *         nextState = FSM_STATE_INACTIVE;
 *     }
 *
 *     return nextState;
 * }
 *
 * static void OnEntryActive(void)
 * {
 *     printf("Exit state ACTIVE\r\n");
 * }
 *
 * void main(void)
 * {
 *     FsmHandle_t fsmHandle;
 *
 *     fsmInit(&fsmHandle, FSM_STATE_INIT);
 *
 *     fsmAdd(&fsmHandle, FSM_STATE_INIT,     StateInit,     onEntryOne,      NULL);
 *     fsmAdd(&fsmHandle, FSM_STATE_INACTIVE, StateInactive, OnEntryInactive, NULL);
 *     fsmAdd(&fsmHandle, FSM_STATE_ACTIVE,   StateActive,   OnEntryActive,   OnExitActive);
 *
 *     while (1)
 *     {
 *         fsmRun(&fsmHandle);
 *     }
 * }
 *
 ********************************************************************************/

#include "stdint.h"

#define FSM_MAX_NR_OF_STATES  (uint8_t)10U

typedef uint8_t FsmStateFunc_t(void);
typedef void FsmOnEntryFunc_t(void);
typedef void FsmOnExitFunc_t(void);

typedef struct
{
    FsmStateFunc_t* stateFunc;
    FsmOnEntryFunc_t* onEntryFunc;
    FsmOnExitFunc_t* onExitFunc;
} FsmStateDef_t;

typedef struct
{
    uint8_t currentState;
    FsmStateDef_t table[FSM_MAX_NR_OF_STATES];
} FsmHandle_t;

int8_t fsmInit(FsmHandle_t* fsmHandle, const uint8_t initState);
int8_t fsmAdd(FsmHandle_t* fsmHandle, const uint8_t state, FsmStateFunc_t stateFunc, FsmOnEntryFunc_t onEntryFunc, FsmOnExitFunc_t onExitFunc);
void fsmRun(FsmHandle_t* fsmHandle);
