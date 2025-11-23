#include "util.h"

/******************************************************************
 * Reads from stdin and fills in the provided buffer with the entered
 * command (without trailing newline).
 *
 * @param buffer pointer to the buffer that will be filled
 * @param size size of the buffer
 *
 * @return true when successfully read from the stdin, false if error or EOF
 */
bool readCommand(char* buffer, size_t size) {
  memset(buffer, 0, size * sizeof(char));
  if (fgets(buffer, size * sizeof(char), stdin) != NULL) {
    buffer[strlen(buffer) - 1] = '\0';
    return true;
  }
  return false;
}

/******************************************************************
 * Predefined function for testing. This is just a wrapper for testing
 * the specific results. You can extend, modify or remove our provided
 * output if you want.
 *
 * @param none
 *
 * @return none
 */
void checkResults()
{

}


/******************************************************************
 * Wrapper function for testing the Board setup.
 *
 * @param none
 *
 * @return none
 */
void checkBoard()
{
  if (process_id == -1)
    exit(-3);
}

/******************************************************************
 * Checks the read- and write capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMrw(int fd)
{
  if (fd == -1)
  {
    exit(-1);
  }
}

/******************************************************************
 * Checks the read-only capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMro(int fd)
{
  if (fd == -1)
  {
    exit(-1);
  }
}

/******************************************************************
 * Wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMPlayer()
{
  // use checkSHMro and checkSHMrw here if you want
}

/******************************************************************
 * Wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMBoard()
{
  // use checkSHMro and checkSHMrw here if you want
}

/******************************************************************
 * Predefined function for testing.
 * Wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPBoard()
{
  if (mmaps.response_ == NULL || mmaps.request_ == NULL ||
      mmaps.locks_ == NULL || mmaps.quizstate_ == NULL)
  {
    exit(-1);
  }
}

/******************************************************************
 * Predefined function for testing.
 * Wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPPlayer()
{
  if (mmaps.response_ == NULL || mmaps.request_ == NULL ||
      mmaps.locks_ == NULL || mmaps.quizstate_ == NULL)
  {
    exit(-1);
  }
}

/******************************************************************
 * Predefined function for testing. Checks if everything was cleaned
 * up properly in both processes. Add your own tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkCleanup()
{
}

/******************************************************************
 * Predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkSetup()
{
  return 0;
}

/******************************************************************
 * Predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgress()
{
  return 0;
}

/******************************************************************
 * Predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressBoard()
{
  return 0;
}

/******************************************************************
 * Predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressPlayer()
{
  return 0;
}


/******************************************************************
 * Programs main function. Here for testing reasons.
 *
 * @param none
 *
 * @return none
 */
int main(void) {
#ifdef START
  return START();
#endif
}
