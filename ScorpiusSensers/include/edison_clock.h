/*
 * define high-resolution clock for Intel Edison real time operation
 *
 *	writer: Qiki	2015/11/2
 *
 */

/*
 * high-resolution clock initialization for real time operation
 * MUST call this function before calling delay_ms()/get_ms() and other
 * real time
 */
int init_clock();

/*
 * blocked delay
 * NOTE: CALL init_clock() FIRST
 * @return 0 if successful
 */
int delay_ms(unsigned long num_ms);

/*
 * get clock count in milliseconds
 *
 * @count	out parameter present system clock count in milliseconds
 * @return 	0 successful, others failed
 */
int get_ms(unsigned long *count);
