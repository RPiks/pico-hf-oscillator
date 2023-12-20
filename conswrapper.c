///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY, PhD
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  conswrapper.c - Serial console commands processing manager.
//
//  DESCRIPTION
//      -
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
//      Rev 0.1   23 Dec 2023   Initial revision.
//
//  PROJECT PAGE
//      https://github.com/RPiks/pico-hf-oscillator
//
//  LICENCE
//      MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Copyright (c) 2023 by Roman Piksaykin
//  
//  Permission is hereby granted, free of charge,to any person obtaining a copy
//  of this software and associated documentation files (the Software), to deal
//  in the Software without restriction,including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/assert.h"

/// @brief Console commands manager. Currently available:
/// @brief HELP                 - Usage.
/// @brief SETFREQ [XXXXXXXX]   - Set oscillator output frequency in Hz.
/// @brief SWITCH [X]           - Switch output to ON or OFF state.
/// @param cmd Ptr to command.
/// @param narg Argument count.
/// @param params Command params, full string.
void ConsoleCommandsWrapper(char *cmd, int narg, char *params)
{
    assert_(cmd);

    if(strstr(cmd, "HELP"))
    {
        printf("\n");
        printf("Pico-hf-oscillator project HELP page\n");
        printf("Copyright (c) 2023 by Roman Piksaykin\n");
        printf("Build date: %s %s\n",__DATE__, __TIME__);
        printf("Project official page: github.com/RPiks/pico-hf-oscillator\n");
        printf("----------------------------------------------------------\n");
        printf("-\n");
        printf("  HELP - this page.\n");
        printf("-\n");
        printf("  SETFREQ [X] - set output frequency in Hz.\n");
        printf("  Example: SETFREQ 14074010 - set output frequency to 14.074010 MHz.\n");
        printf("-\n");
        printf("  SWITCH [X] - switch generation to ON or OFF state.\n");
        printf("  Example: SWITCH ON - switch generation to ON state.\n");
    } else if(strstr(cmd, "SETFREQ"))
    {

    } else if(strstr(cmd, "SWITCH"))
    {

    }
}
