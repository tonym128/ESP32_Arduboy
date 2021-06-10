struct Entity: Simple
{
	uint8_t width = 0, height = 0;

	uint8_t frameCount = 0;
	uint8_t curFrame = 0;

	float angle = 0;
	float speed = 0;
	uint8_t maxSpeed = 2;

	bool contain = false;

	void physics()
	{
		// apply momentum according to the current angle
		x += speed * cos(angle * 1000 / 57296);
		y -= speed * sin(angle * 1000 / 57296);

		// contain to the screen
		if (contain)
		{
			if (x < 0)
			{
				x = 0;
			}
			else if (x + width > LEVEL_SIZE)
			{
				x = LEVEL_SIZE - width;
			}

			// contain to the screen
			if (y < 0)
			{
				y = 0;
			}
			else if (y + height > LEVEL_SIZE)
			{
				y = LEVEL_SIZE - height;
			}
		}
	}
};
