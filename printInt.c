static void UART_PrintUInt(unsigned int v)
{
    // 0..1023 için basit decimal yazdırma
    char buf[6];
    int i = 0;

    if(v == 0) { UART_PutC('0'); return; }

    while(v > 0 && i < 5)
    {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }
    while(i--) UART_PutC(buf[i]);
}
