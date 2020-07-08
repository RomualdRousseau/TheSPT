#define SERIAL_BUFFER_SIZE  256
#define SERIAL_MAX_ARGS     100

#define AT                0
#define AT_LIST_MODEL     1
#define AT_GET_MODEL      2
#define AT_LOAD_MODEL     3   
#define AT_SAVE_MODEL     4
#define AT_RENAME_MODEL   5
#define AT_READ_MIXER     6
#define AT_WRITE_MIXER    7
#define AT_READ_TRIM      8
#define AT_WRITE_TRIM     9
#define AT_READ_FUNC      10
#define AT_WRITE_FUNC     11
#define AT_READ_BATTERY   12
#define AT_READ_COMMAND   13
#define AT_UNKNOWN        255

void at_version();
void at_list_model();
void at_get_model();
void at_load_model(int model);
void at_save_model();
void at_rename_model(char* newname);
void at_read_mixer();
void at_write_mixer(char** args);
void at_read_trim();
void at_write_trim(char** args);
void at_read_func();
void at_write_func(char** args);
void at_read_battery();
void at_read_command();

class Serial_
{
  public:
    void init();

    void reset();

    void process_at_command();

  private:
    char buffer[SERIAL_BUFFER_SIZE];
    int buffer_idx;

    int tokenize(char* s, char** argv, int* argc);

    void accept(int token, char** argv, int argc);
};

void Serial_::init()
{
  // Serial.begin(57600);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void Serial_::reset()
{
  buffer_idx = 0;
}

void Serial_::process_at_command()
{
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      buffer[buffer_idx] = 0;
      
      char* argv[SERIAL_MAX_ARGS + 1];
      int argc = 0;
      int cmd = tokenize(buffer, argv, &argc);
      accept(cmd, argv, argc);

      buffer_idx = 0;
    }
    else if (c != '\r')
    {
      buffer[buffer_idx] = c;
      buffer_idx = (buffer_idx + 1) % SERIAL_BUFFER_SIZE;
    }
  }
}

int Serial_::tokenize(char* s, char** argv, int* argc)
{
  char** p = argv;
  int n = 0;

  char* token = strtok(s, "=,");
  while (token != NULL && n < SERIAL_MAX_ARGS)
  {
    p[n++] = token;
    token = strtok(NULL, "=,");
  }
  *argc = n;
  
  if (n == 0) {
    return AT_UNKNOWN;
  }

  if (strcmp(argv[0], "AT") == 0)
  {
    return AT;
  }
  else if (strcmp(argv[0], "AT+LSMDL") == 0)
  {
    return AT_LIST_MODEL;
  }
  else if (strcmp(argv[0], "AT+GMDL") == 0)
  {
    return AT_GET_MODEL;
  }
  else if (strcmp(argv[0], "AT+LMDL") == 0)
  {
    return AT_LOAD_MODEL;
  }
  else if (strcmp(argv[0], "AT+SMDL") == 0)
  {
    return AT_SAVE_MODEL;
  }
  else if (strcmp(argv[0], "AT+RNMDL") == 0)
  {
    return AT_RENAME_MODEL;
  }
  else if (strcmp(argv[0], "AT+RMXR") == 0)
  {
    return AT_READ_MIXER;
  }
  else if (strcmp(argv[0], "AT+WMXR") == 0)
  {
    return AT_WRITE_MIXER;
  }
  else if (strcmp(argv[0], "AT+RTRM") == 0)
  {
    return AT_READ_TRIM;
  }
  else if (strcmp(argv[0], "AT+WTRM") == 0)
  {
    return AT_WRITE_TRIM;
  }
  else if (strcmp(argv[0], "AT+RFUN") == 0)
  {
    return AT_READ_FUNC;
  }
  else if (strcmp(argv[0], "AT+WFUN") == 0)
  {
    return AT_WRITE_FUNC;
  }
  else if (strcmp(argv[0], "AT+RBAT") == 0)
  {
    return AT_READ_BATTERY;
  }
  else if (strcmp(argv[0], "AT+RCOM") == 0)
  {
    return AT_READ_COMMAND;
  }
  else
  {
    return AT_UNKNOWN;
  }
}

void Serial_::accept(int token, char** argv, int argc)
{
  if (token == AT)
  {
    at_version();
  }

  if (token == AT_LIST_MODEL)
  {
    at_list_model();
  }

  if (token == AT_GET_MODEL)
  {
    at_get_model();
  }

  if (token == AT_LOAD_MODEL && argc == 1+1)
  {
    at_load_model(atoi(argv[1]));
  }

  if (token == AT_SAVE_MODEL)
  {
    at_save_model();
  }

  if (token == AT_RENAME_MODEL && argc == 1+1)
  {
    at_rename_model(argv[1]);
  }

  if (token == AT_READ_MIXER)
  {
    at_read_mixer();
  }

  if (token == AT_WRITE_MIXER && argc == 1+1+8)
  {
    at_write_mixer(argv);
  }

  if (token == AT_READ_TRIM)
  {
    at_read_trim();
  }

  if (token == AT_WRITE_TRIM && argc == 1+8)
  {
    at_write_trim(argv);
  }

  if (token == AT_READ_FUNC)
  {
    at_read_func();
  }

  if (token == AT_WRITE_FUNC && argc == 1+8)
  {
    at_write_func(argv);
  }

  if (token == AT_READ_BATTERY)
  {
    at_read_battery();
  }

  if (token == AT_READ_COMMAND)
  {
    at_read_command();
  }
}
