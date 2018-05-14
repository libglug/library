namespace glug
{

struct library {
  const char *name;
  bool loaded;
  void *dll;
};

} // namespace glug
