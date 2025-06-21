/*
 * scicomm.c
 *
 *  Created on: 13 de jun de 2025
 *      Author: Guilherme Márcio Soares
 */
#include "board.h"
#include "device.h"
#include "scicomm.h"

#define NUM_PONTOS_WAVEFORM 500


int protocolReceiveInt(unsigned int sci_base)
{
    uint16_t buffer[INT_SIZE];
    SCI_readCharArray(sci_base, buffer, INT_SIZE);
    return (buffer[0] | (buffer[1] << 8U));
}

void protocolSendInt(unsigned int sci_base,int data)
{
    uint16_t txBuf[INT_SIZE];
    txBuf[0] = (uint16_t)(data & 0x00FF);
    txBuf[1] = (uint16_t)((data >> 8U) & 0x00FF);

    SCI_writeCharArray(sci_base, txBuf, INT_SIZE);
}


void protocolReceiveWaveForm(unsigned int sci_base, float waveform[])
{
    int num_pontos = protocolReceiveInt(sci_base); // Pega a quantidades de pontos que será enviada

    if (num_pontos > NUM_PONTOS_WAVEFORM) { // Verifica o tamanho do vetor ja criado
        num_pontos = NUM_PONTOS_WAVEFORM;
    }

    int16_t parte_inteira;
    int16_t parte_decimal;

    for (int i = 0; i < num_pontos; i++) {
        parte_inteira = protocolReceiveInt(sci_base);   // Pega a parte inteira do numero
        parte_decimal = protocolReceiveInt(sci_base);   // Pega a parte decimal do numero

        waveform[i] = (float)parte_inteira + ((float)parte_decimal / 10000.0f); // Faz as contas para juntar a parte decimal com a inteira
    }
}

void protocolSendWaveForm(unsigned int sci_base, float waveform[])
{
    int16_t parte_inteira;
    int16_t parte_decimal;
    for (int i = 0; i < NUM_PONTOS_WAVEFORM; i++)
    {
        // Divide a parte inteira e a parte decimal
        parte_inteira = (int16_t)waveform[i];
        parte_decimal = (int16_t)((fabsf(waveform[i] - parte_inteira)) * 10000.0f);

        // Aplica o mesmo sinal da parte inteira à parte decimal
        if (waveform[i] < 0)
        {
            parte_decimal = -parte_decimal;
        }

        uint16_t txBuf[4];

        txBuf[0] = (uint16_t)(parte_inteira & 0x00FF);
        txBuf[1] = (uint16_t)((parte_inteira >> 8U) & 0x00FF);
        txBuf[2] = (uint16_t)(parte_decimal & 0x00FF);
        txBuf[3] = (uint16_t)((parte_decimal >> 8U) & 0x00FF);

        SCI_writeCharArray(sci_base, txBuf, 4);  // 4 bytes = 2 int16_t
    }
}



