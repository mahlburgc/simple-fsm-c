/********************************************************************************
 * @file           : fsm.c
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
 ********************************************************************************/

#include "fsm.h"
#include "stddef.h"

/**
 * @brief Set the init state of the FSM (state that is executed first).
 *        Should be called first to initialize the  fsm table and set
 *        the initial state.
 *
 * @param fsmHandle - fsm instance
 * @param initState - state that should be started first
 * @return  0 - initState was set correctly
 *         -1 - initState exeed max number of states
 */
int8_t fsmInit(FsmHandle_t* fsmHandle, const uint8_t initState)
{
    int8_t stateAdded = -1;

    for (uint8_t i = 0; i < FSM_MAX_NR_OF_STATES; i++)
    {
        fsmHandle->table[i].stateFunc = NULL;
        fsmHandle->table[i].onEntryFunc = NULL;
        fsmHandle->table[i].onExitFunc= NULL;
    }

    if (initState < FSM_MAX_NR_OF_STATES)
    {
        fsmHandle->currentState = initState;
        stateAdded = 0;
    }
    else
    {
        fsmHandle->currentState = 0;
    }
    return stateAdded;
}

/**
 * @brief Register a state to the fsm.
 *
 * @param fsmHandle - fsm instance
 * @param state - state index
 * @param stateFunc - function pointer that is called if state is executed
 * @param onEntryFunc - function pointer that is called when entering a new state
 * @param onExitFunc - function pointer that is called when leaving a state
 * @return  0 - state added successfully
 *         -1 - state could not be added
 */
int8_t fsmAdd(FsmHandle_t* fsmHandle, const uint8_t state, FsmStateFunc_t stateFunc, FsmOnEntryFunc_t onEntryFunc, FsmOnExitFunc_t onExitFunc)
{
    int8_t stateAdded = -1;

    if ((state < FSM_MAX_NR_OF_STATES) && (stateFunc != NULL))
    {
        fsmHandle->table[state].stateFunc = stateFunc;
        fsmHandle->table[state].onEntryFunc = onEntryFunc;
        fsmHandle->table[state].onExitFunc = onExitFunc;
        stateAdded = 0;
    }
    return stateAdded;
}

/**
 * @brief FSM Core - execute the fsm.
 *
 * @param fsmHandle - fsm instance
 */
void fsmRun(FsmHandle_t* fsmHandle)
{
    if (NULL != fsmHandle->table[fsmHandle->currentState].stateFunc)
    {
        uint8_t stateNext = fsmHandle->table[fsmHandle->currentState].stateFunc();

        if (fsmHandle->currentState != stateNext)
        {
            if (NULL != fsmHandle->table[fsmHandle->currentState].onExitFunc)
            {
                fsmHandle->table[fsmHandle->currentState].onExitFunc();
            }
            if (NULL != fsmHandle->table[stateNext].onEntryFunc)
            {
                fsmHandle->table[stateNext].onEntryFunc();
            }
            fsmHandle->currentState = stateNext;
        }
    }
}

