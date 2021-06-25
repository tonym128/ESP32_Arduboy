struct Crate: Solid
{
	Crate() = default;

	Crate(Point spawn)
	{
		x = spawn.x, y = spawn.y;
		width = 16, height = 16;
		cbox_conf = {.x = 0, .y = 10, .width = 16, .height = 6};
		type = 'C';
	}

	void update()
	{
		updateCbox();
	}

	void draw() const
	{
		sketch(CRATE);
	}
};
