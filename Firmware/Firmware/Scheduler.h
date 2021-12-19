#define TASK_IDLE           0

#define DEFINE_TASK_FREQ(f) (1000000 / f)

struct Task
{
  void (*callback)(float dt);
  unsigned long deadline;
  unsigned long last_exec_time;
  unsigned long last_duration_time;
  unsigned long accumulated_time;
};

class Scheduler
{
  public:
    void calibrate(Task* tasks, int sample_count);
    
    void register_tasks(Task* tasks, int count);

    void debug_tasks();
    
    void run_mainloop();

  private:
    Task*         _tasks;
    int           _task_count;
    unsigned long _run_count;
    unsigned long _time_quantum = HAL_SCHEDULER_QUANTUM_TIME;
};

void Scheduler::calibrate(Task* task, int sample_count)
{
#ifdef DEBUG
  Serial.print("Calibrating scheduler ... ");
#endif

  unsigned long start_time = micros();
  for (int i = 0; i < sample_count; i++)
  {
    task->callback(0);
  }
  _time_quantum = 1 + (micros() - start_time) / sample_count;

#ifdef DEBUG
  Serial.print(_time_quantum);
  Serial.println("us");
#endif
}


void Scheduler::register_tasks(Task* tasks, int count)
{
  _tasks = tasks;
  _task_count = count;
  _run_count = 0;

  unsigned long current_time = micros();
  for (int i = 0; i < _task_count; i++)
  {
    _tasks[i].last_exec_time = current_time;
    _tasks[i].last_duration_time = 0;
    _tasks[i].accumulated_time = 0;
  }
}

void Scheduler::debug_tasks()
{
#ifdef DEBUG
  unsigned long sum_duration_time = 0;
  unsigned long sum_avg_duration_time = 0;
  for (int i = 0; i < _task_count; i++) {
    sum_duration_time += _tasks[i].last_duration_time;
    sum_avg_duration_time += _tasks[i].accumulated_time / _run_count;
  }

  Serial.println("=== Tasks");
  for (int i = 0; i < _task_count; i++) {
    Serial.print("\t");
    Serial.print(i);
    Serial.print("\t");
    Serial.print(1000 / (_tasks[i].deadline / 1000));
    Serial.print("Hz\t");
    Serial.print(_tasks[i].last_duration_time / 1000);
    Serial.print("ms\t");
    Serial.print(_tasks[i].accumulated_time / _run_count / 1000);
    Serial.print("ms\t");
    Serial.print(_tasks[i].last_duration_time * 100 / sum_duration_time);
    Serial.print("%\t"); 
    Serial.print((_tasks[i].accumulated_time / _run_count) * 100 / sum_avg_duration_time);
    Serial.println("%");
  }
#endif
}

void Scheduler::run_mainloop()
{
  unsigned long current_time = micros();

  for (int i = 0; i < _task_count; i++)
  {
    unsigned long t = current_time - _tasks[i].last_exec_time;
    if (t >= _tasks[i].deadline)
    {
      _tasks[i].last_exec_time = micros();
      _tasks[i].callback(t * 0.001);
      _tasks[i].last_duration_time = micros() - _tasks[i].last_exec_time;
      _tasks[i].accumulated_time += _tasks[i].last_duration_time;
    }
  }

  _run_count++;

  unsigned long t = micros() - current_time;
  if (t < _time_quantum)
  {
    delayMicroseconds(_time_quantum - t);
  }
}
