class Test : ITest
{
	private int tickCount = 0;

	void tick(const float value)
	{
		tickCount++;
	}
}

void main()
{
	ITest@ test = Test();
	setTestInstance(test);
}
