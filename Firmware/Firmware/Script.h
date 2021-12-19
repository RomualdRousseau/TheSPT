struct Scriptlet
{
  void (*callback)();
  int command;
  int min;
  int max;
  bool bounce;
};

class Script
{
  public:
    void init();

    void register_scriptlets(Scriptlet* scriptlets, int count);

    void run();

    void run_one_scriptlet(Scriptlet* scriptlet, int param);

  private:
    Scriptlet*    _scriptlets;
    int           _scriptlet_count;
};

void Script::init()
{
#ifdef DEBUG
  Serial.println("Initializing script engine ... ");
#endif
}

void Script::register_scriptlets(Scriptlet* scriptlets, int count)
{
  _scriptlets = scriptlets;
  _scriptlet_count = count;

  for (int i = 0; i < _scriptlet_count; i++) {
    _scriptlets[i].bounce = false;
  }
}


void Script::run_one_scriptlet(Scriptlet* scriptlet, int param)
{
  if (!scriptlet->bounce) {
    if (scriptlet->min <= param && param <= scriptlet->max) {
      scriptlet->callback();
      scriptlet->bounce = true;
    }
  } else {
    if (scriptlet->min > param || param > scriptlet->max) {
      scriptlet->bounce = false;
    }
  }
}
