#include "board.h"
#include "assert.h"

struct {
  char* invalid_command_;
  char* invalid_answer_argument_;
  char* welcome_;
  char* bye_;
  char* game_not_yet_started_;
  char* game_already_started_;
} messages = {
    .invalid_command_ = "Invalid command...\n",
    .invalid_answer_argument_ = "Invalid answer argument...\n",
    .welcome_ = "Welcome to the quiz game!\n",
    .bye_ = "Until next time!\n",
    .game_not_yet_started_ = "The game has not yet started!\n",
    .game_already_started_ = "The game has already started!\n",
};

// Buffer for dynamic string concatenation
char dynamic_response_buffer[RESPONSE_MAX_LENGTH];

// TODO Student START
// you can add your own global variables here if needed
// (do not use the same names in Player and Board because of testing reasons)

// TODO Student END

/******************************************************************
 * Main routine that contains the game loop. After the initial setup,
 * requests are handled and answers are published to the Player.
 * Stops when the exit command is received. Initialize everything needed
 * in here. Checks in this function must not be relocated.
 * Do NOT use any synchronization mechanisms outside of the game loop.
 *
 * @param none
 *
 * @return predefined exit codes
 */
int startboard()
{
  initSharedMemoriesBoard();
  checkSHMBoard();
  initMemoryMappingsBoard();
  checkMMAPBoard();

  memset(mmaps.quizstate_, 0, sizeof(shmquizsstate));

  // TODO Student START
  // send welcome message to player
  strcpy(mmaps.response_->message, messages.welcome_);
  sem_post(&mmaps.locks_->board_init_done);

  // TODO Student END

  do
  {
    // TODO Student START
    // handle incoming commands and respond accordingly
    sem_post(&mmaps.locks_->board_ready_for_new_command);


    // wait for player to enter input
    sem_wait(&mmaps.locks_->player_input);
    
    // check the command does this need to be flipped
    strcpy(mmaps.response_->message ,cmdHandler(mmaps.request_));
    
    // release the player process
    sem_post(&mmaps.locks_->board_check_command);





    // TODO Student END

  } while (strcmp(mmaps.request_->message, CMD_EXIT) != 0);

  closeMmapingsBoard();
  checkCleanup();

  return 0;

}

/******************************************************************
 * Initializes the Boards shared objects and resizes them. Make
 * sure to only give the permissions the Board needs. But please
 * use MODERW, for compatibility with the testsystem.
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesBoard()
{
  // TODO Student START
  fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
  fds.fd_shm_quizstate_ = shm_open(SHM_NAME_QUIZSTATE, FLAGS_SHM_READWRITE, MODERW);
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READONLY, MODERW);
  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READWRITE, MODERW);

  if (fds.fd_shm_quizstate_ == -1 || fds.fd_shm_locks_ == -1 || 
  fds.fd_shm_request_ == -1 || fds.fd_shm_response_ == -1)
  {
    printf("failed shm_open\n");
  }

  if (ftruncate(fds.fd_shm_locks_, 4096) == -1)
  {
    printf("failed ftruncate1 b\n");
  }
  if (ftruncate(fds.fd_shm_quizstate_, 4096) == -1)
  {
    printf("failed ftruncate2 b\n");
  }
  /*if (ftruncate(fds.fd_shm_request_, 4096) == -1)
  {
    printf("failed ftruncate3 b\n");
  }*/
  if (ftruncate(fds.fd_shm_response_, 4096) == -1)
  {
    printf("failed ftruncate4 b\n");
  }


  // TODO Student END
}

/******************************************************************
 * Maps the shared objects to the virtual memory space of the Board.
 * Don't do anything else in this function.
 *
 * @param none
 *
 * @return none
 */
void initMemoryMappingsBoard()
{
  if (checkProgressBoard())
  {
    return;
  }

  // TODO Student START

  mmaps.locks_ = (shmlocks*) mmap(0, 4096, PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);
  mmaps.quizstate_ = (shmquizsstate*) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_quizstate_, 0);
  mmaps.request_ = (shmrequest*) mmap(0, 4096, PROT_READ, MAP_SHARED, fds.fd_shm_request_, 0);
  mmaps.response_ = (shmresponse*) mmap(0, 4096, PROT_WRITE, MAP_SHARED, fds.fd_shm_response_, 0);

  // TODO Student END
}

/******************************************************************
 * Removes all mmaps and shared objects as seen from the Board.
 * This part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsBoard()
{
  if (checkProgressBoard())
  {
    return;
  }

  // TODO Student START

  munmap(mmaps.locks_, 4096);
  munmap(mmaps.quizstate_, 4096);
  munmap(mmaps.request_, 4096);
  munmap(mmaps.response_, 4096);

  close(fds.fd_shm_locks_);
  close(fds.fd_shm_quizstate_);
  close(fds.fd_shm_request_);
  close(fds.fd_shm_response_);

  shm_unlink(SHM_NAME_LOCKS);
  shm_unlink(SHM_NAME_QUIZSTATE);
  shm_unlink(SHM_NAME_REQUEST);
  shm_unlink(SHM_NAME_RESPONSE);


  // TODO Student END
}

/******************************************************************
 * Handle the incoming requests and return the appropiate answer.
 *
 * @param shmrequest the pointer to the request object
 *
 * @return char* the answer to the request
 */
char *cmdHandler(shmrequest *request)
{
  // TODO Student START
  // Respond to the incoming commands and set the quizstate accordingly.
  // Make sure to use the helper functions defined in board.h!

  if (isCommand(CMD_START, request))
  {
    // ...
    if (mmaps.quizstate_->game_active_ == true)
    {
      return messages.game_already_started_;
    } else
    {
      
      setupQuizstate(mmaps.quizstate_);
      return generateQuestionText(mmaps.quizstate_);
    }

  }
  else if (isCommand(CMD_ANSWER, request))
  {
    // ...
    if (mmaps.quizstate_->game_active_ == false)
    {
      return messages.game_not_yet_started_;
    }
    char answer = getAnswerArgument(mmaps.request_);
    if (!(answer >= 'a' && answer < 'a' + NUM_ANSWERS_PER_QUESTION))
    {
      return messages.invalid_answer_argument_;
    } else
    {
      answerCurrentQuestion(mmaps.quizstate_, answer);
      
      if (mmaps.quizstate_->game_active_ == false)
      {
        return generateResultsText(mmaps.quizstate_);
      }
      
      return generateQuestionText(mmaps.quizstate_);
    }
    
    
  }
  else if (isCommand(CMD_SKIP, request))
  {
    // ...
    if (mmaps.quizstate_->game_active_ == false)
    {
      return messages.game_not_yet_started_;
    } else
    {
      skipCurrentQuestion(mmaps.quizstate_);
      if (mmaps.quizstate_->game_active_ == false)
      {
        return generateResultsText(mmaps.quizstate_);
      }
      
      return generateQuestionText(mmaps.quizstate_);
    }
  }
  else if (isCommand(CMD_EXIT, request))
  {
    return messages.bye_;
  } else
  {
    return messages.invalid_command_;
  }
  

  // TODO Student END

  return messages.invalid_command_;
}

/******************************************************************
 * Checks if the command is the same as the one in the request.
 *
 * @param const char* the command to check
 * @param const shmrequest* the pointer to the request object
 *
 * @return bool true if the command is the same
 */
bool isCommand(const char* cmd, const shmrequest* request)
{
  return strncmp(cmd, request->message, strlen(cmd)) == 0;
}

/******************************************************************
 * Extracts the answer argument from the request.
 *
 * @param request the pointer to the request object
 *
 * @return the answer argument or 0 if the argument is invalid
 */
char getAnswerArgument(const shmrequest* request)
{
  char answer = request->message[strlen(CMD_ANSWER) + 1];
  if (answer >= 'a' && answer < 'a' + NUM_ANSWERS_PER_QUESTION) {
    return answer;
  }
  return 0;
}

/******************************************************************
 * Initializes the quizstate object with the question ids and
 * resets the answers and the current question index.
 *
 * @param quizstate the pointer to the quizstate object
 *
 * @return none
 */
void setupQuizstate(shmquizsstate* quizstate)
{
  assert(!quizstate->game_active_ && "Did you forget to check something? ;)");
#ifdef SEED
  srand(SEED);
#else
  srand(time(NULL));
#endif
  for (int i = 0; i < NUM_QUESTIONS_PER_GAME;) {
    char qid = rand() % NUM_QUESTIONS_IN_POOL;
    bool exists = false;
    for (int j = 0; j < i; j++) {
      if (quizstate->question_ids_[j] == qid) {
        exists = true;
        break;
      }
    }
    if (exists) {
      continue;
    }
    quizstate->question_ids_[i] = qid;
    i++;
  }
  memset(quizstate->answers_, 0, NUM_QUESTIONS_PER_GAME * sizeof(char));
  quizstate->current_question_index_ = 0;
  quizstate->game_active_ = true;
}

/******************************************************************
 * Generates the text for the current question and prints it to stdout
 *
 * @param quizstate the pointer to the quizstate object
 *
 * @return the text for the current question
 */
char* generateQuestionText(const shmquizsstate* quizstate)
{
  assert(quizstate->game_active_ && "Did you forget to check something? ;)");
  memset(dynamic_response_buffer, 0, RESPONSE_MAX_LENGTH);
  question_t* question = &QUESTION_POOL[quizstate->question_ids_[quizstate->current_question_index_]];
  snprintf(dynamic_response_buffer, RESPONSE_MAX_LENGTH, "Question %d/%d: %s\n", quizstate->current_question_index_ + 1,
           NUM_QUESTIONS_PER_GAME, question->question_);
  for (int i = 0; i < NUM_ANSWERS_PER_QUESTION; i++) {
    size_t len = strlen(dynamic_response_buffer);
    snprintf(dynamic_response_buffer + len, RESPONSE_MAX_LENGTH - len, "%c. %s\n", 'a' + i,
             question->answers_[i]);
  }
  return dynamic_response_buffer;
}

/******************************************************************
 * Generates the text for the results, and prints it to stdout.
 *
 * @param quizstate the pointer to the quizstate object
 *
 * @return the text for the results
 */
char* generateResultsText(const shmquizsstate* quizstate)
{
  assert(!quizstate->game_active_ && "Did you forget to set something? ;)");
  assert(quizstate->current_question_index_ == NUM_QUESTIONS_PER_GAME && "Not so fast! Check again if you correctly answered/skipped all questions.");

  memset(dynamic_response_buffer, 0, RESPONSE_MAX_LENGTH);
  int correct_answers = 0;
  int skipped_answers = 0;
  for (int i = 0; i < NUM_QUESTIONS_PER_GAME; i++) {
    assert(quizstate->question_ids_[i] >= 0 && quizstate->question_ids_[i] < NUM_QUESTIONS_IN_POOL && "Did you set up the quiz correctly?");
    assert(quizstate->answers_[i] != 0 && "Not so fast! Check again if you correctly answered/skipped all questions.");
    assert((quizstate->answers_[i] == '_' || (quizstate->answers_[i] >= 'a' && quizstate->answers_[i] < 'a' + NUM_ANSWERS_PER_QUESTION)) && "Did you validate the answer argument?");
    question_t* question = &QUESTION_POOL[quizstate->question_ids_[i]];
    if (quizstate->answers_[i] == question->correct_answer_) {
      correct_answers++;
    }
    if (quizstate->answers_[i] == '_') {
      skipped_answers++;
    }
  }
  snprintf(dynamic_response_buffer, RESPONSE_MAX_LENGTH,
    "Quiz finished!\n  Correct answers:   %d/%d\n  Skipped questions: %d/%d\n",
    correct_answers, NUM_QUESTIONS_PER_GAME, skipped_answers, NUM_QUESTIONS_PER_GAME);
  return dynamic_response_buffer;
}

/**
 * Answers the current question with the given answer. If it was the last
 * question, the game is set inactive, so CHECK for game_active_==false after using
 * this function.
 *
 * @param quizstate the pointer to the quizstate object
 * @param answer the answer to the current question. Is not validated, so make sure
 *  it is correct beforehand
 */
void answerCurrentQuestion(shmquizsstate* quizstate, char answer)
{
  assert(quizstate->game_active_ && "Did you forget to check something? ;)");
  quizstate->answers_[quizstate->current_question_index_] = answer;
  quizstate->current_question_index_++;
  if (quizstate->current_question_index_ == NUM_QUESTIONS_PER_GAME) {
    quizstate->game_active_ = false;
  }
}

/**
 * Skips the current question. If it was the last question, the game is set inactive,
 * so CHECK for game_active_==false after using this function.
 *
 * @param quizstate the pointer to the quizstate object
 */
void skipCurrentQuestion(shmquizsstate* quizstate)
{
  answerCurrentQuestion(quizstate, '_');
}
