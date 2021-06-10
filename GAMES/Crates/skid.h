struct Skid: Entity
{
	Skid() = default;

	Skid(float X, float Y, uint8_t CurFrame)
	{
		x = X, y = Y;

		curFrame = CurFrame;

		ttl = 10;
	}

	void update()
	{
		ttl--;
	}

	void draw() const
	{
		sketch(SKID, curFrame);
	}
};
