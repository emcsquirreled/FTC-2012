typedef struct
{
  string question;
  string answers[20];
} TStringArgs;


int waitForRelease(TButtons watchedButton)
{
  while(watchedButton != -1)
  {
    watchedButton = nNxtButtonPressed;
  }

  return 0;
}

int formatParams(TStringArgs &args, string &returnStr)
{
  if(strncmp(args.question, "", strlen(args.question)) == 0) return -1;
  if(strncmp(args.answers[0], "", strlen(args.answers[0])) == 0) return -1;
  return 0;
}
