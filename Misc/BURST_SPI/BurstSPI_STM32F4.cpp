#include <cstdint>
#include <stdint.h>
#if defined(TARGET_STM32F4)
#include "BurstSPI.h"
 
void BurstSPI::fastWrite(int data) {
    
    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance;
    
    // Check if data is transmitted
    while ((spi->SR & SPI_SR_TXE) == 0);
    spi->DR = data;

    while ((spi->SR & SPI_SR_BSY) == 0); // wait until BSY
    while ((spi->SR & SPI_SR_BSY) != 0); // wait until finished with BSY
}


void BurstSPI::fastWrite_three_byte(int data) {

    uint8_t reg_data_buf[3];
    reg_data_buf[0] = ((0xFF0000 & data) >> 16);
    reg_data_buf[1] = ((0xFF00 & data) >> 8);
    reg_data_buf[2] = (0xFF & data);

    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance;

    for(int i = 0; i < 3; i++){
        // Check if data is transmitted
        while ((spi->SR & SPI_SR_TXE) == 0);
        spi->DR = reg_data_buf[i];
    }

    while ((spi->SR & SPI_SR_TXE) == 0);
    // while ((spi->SR & SPI_SR_BSY) == 0); // wait until BSY
    while ((spi->SR & SPI_SR_BSY) != 0); // wait until finished with BSY
}


uint16_t BurstSPI::fastRead( int reg ) {

    uint16_t rtn_val = 0;
    uint16_t rtn_val_1 = 0;
    uint16_t rtn_val_2 = 0;

    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance;

    // __disable_irq();  // disable all interrupts
    // 1st ADDRESS BYTE
    while ((spi->SR & SPI_SR_TXE) == 0);
    spi->DR = reg;
    while ((spi->SR & SPI_SR_TXE) == 0);

    // 1st DATA BYTE
    while ((spi->SR & SPI_SR_TXE) == 0);
    clearRX();
    spi->DR = 0xFF;
    // while ((spi->SR & SPI_SR_TXE) == 0);
    while ((spi->SR & SPI_SR_RXNE) == 0); 
    // rtn_val |= (spi->DR << 8);
    rtn_val_1 = (spi->DR);

    // 2nd DATA BYTE
    while ((spi->SR & SPI_SR_TXE) == 0);
    clearRX();
    spi->DR = 0xFF;
    // while ((spi->SR & SPI_SR_TXE) == 0);
    while ((spi->SR & SPI_SR_RXNE) == 0); 
    // rtn_val |= (spi->DR);
    rtn_val_2 = (spi->DR);

    // WAIT FOR TRANSACTION TO FINISH
    while ((spi->SR & SPI_SR_BSY) != 0);
    // __enable_irq();  // disable all interrupts
    rtn_val = (rtn_val_1 << 8) | (rtn_val_2);
    return rtn_val;

    // // Write the the register address
    // while ((spi->SR & SPI_SR_TXE) == 0);
    // spi->DR = reg;
    // while ((spi->SR & SPI_SR_TXE) == 0);

    // for(int i = 2; i > 0; i--){

    //     // Quickly clear the RX buffer then write the 0xFF to initiate the read
    //     while (spi->SR & SPI_SR_BSY){   
    //         // Check RX buffer readable
    //         while ((spi->SR & SPI_SR_RXNE) == 0);
    //         int dummy = spi->DR;
    //     }

    //     // write null 0xFF for reading
    //     spi->DR = 0xFF;
    //     while ((spi->SR & SPI_SR_TXE) == 0);
        
    //     // Check RX buffer readable
    //     while ((spi->SR & SPI_SR_RXNE) == 0);

    //     // Read and save data 
    //     rtn_val |= (spi->DR << 8*(i-1));
    //     // while ((spi->SR & SPI_SR_BSY) != 0);
    // }

    // while ((spi->SR & SPI_SR_BSY) != 0);
    // return rtn_val;

}


void BurstSPI::clearRX( void ) {
    //Check if the RX buffer is busy
    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance;

    //While busy, keep checking
    while (spi->SR & SPI_SR_BSY){   
        // Check RX buffer readable
        while ((spi->SR & SPI_SR_RXNE) == 0);
        int dummy = spi->DR;
    }
}
#endif

