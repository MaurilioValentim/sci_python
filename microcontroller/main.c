//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"

#define NUM_PONTOS_WAVEFORM 500

volatile Protocol_Header_t g_prot_header = {CMD_NONE,0};
volatile int g_dado;
float waveform[NUM_PONTOS_WAVEFORM];
//
// Fun��o Principal
//
void main(void)
{
    // Inicializa��o do dispositivo
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();

    // Habilita interrup��es globais e de tempo real
    EINT;
    ERTM;

    while (1)
    {
        if (g_prot_header.cmd != CMD_NONE)
        {
            switch (g_prot_header.cmd)
            {
                case CMD_RECEIVE_INT:
                    g_dado = protocolReceiveInt(SCI0_BASE);
                    break;

                case CMD_SEND_INT:
                    protocolSendInt(SCI0_BASE, g_dado);
                    break;

                case CMD_RECEIVE_WAVEFORM:
                    protocolReceiveWaveForm(SCI0_BASE, waveform);
                    break;

                case CMD_SEND_WAVEFORM:
                    protocolSendWaveForm(SCI0_BASE, waveform);
                    break;
            }

            // Limpa status de interrup��o e reseta comando
            SCI_clearInterruptStatus(SCI0_BASE, SCI_INT_RXFF);
            g_prot_header.cmd = CMD_NONE;
        }
    }
}

//
// Rotina de Interrup��o da SCI (Recep��o)
//
__interrupt void INT_SCI0_RX_ISR(void)
{
    uint16_t header[PROTOCOL_HEADER_SIZE];
    uint16_t cmd;

    SCI_readCharArray(SCI0_BASE, header, PROTOCOL_HEADER_SIZE);
    cmd = header[0];
    g_prot_header.data_len = header[1] | (header[2] << 8);
    g_prot_header.cmd = (cmd < CMD_COUNT)? (SCI_Command_e)cmd : CMD_NONE;

    Interrupt_clearACKGroup(INT_SCI0_RX_INTERRUPT_ACK_GROUP);
}
