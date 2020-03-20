namespace
{
  class S
  {
  public:
    static S& getInstance()
    {
      static S instance;
      return instance;
    }
  private:
    S() {};
    S(S const&);
    void operator=(S const&);
  };
}