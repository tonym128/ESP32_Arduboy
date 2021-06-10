struct Drop: Solid
{
	Drop() = default;

	Drop(Point spawn)
	{
		x = spawn.x, y = spawn.y;
		width = 16, height = 16;
		cbox_conf = {.x = 0, .y = 8, .width = 16, .height = 8};
		type = 'D';
	}

	void update()
	{
		updateCbox();
	}

	void draw() const
	{
		sketch(DROP);
	}
};
