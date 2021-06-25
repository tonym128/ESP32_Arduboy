struct Solid: Entity
{
	char type = 'G'; // a generic type indicator
	uint8_t damage = 0;

	Rect cbox_conf = {.x = 0, .y = 0, .width = width, .height = height}; // used for cbox offset and size
	Rect cbox; // used to actually calculate collisions

	void updateCbox()
	{
		// TODO does modifying like this allocate memory?
		cbox = {.x = (int16_t)x + cbox_conf.x, .y = (int16_t)y + cbox_conf.y, .width = cbox_conf.width, .height = cbox_conf.height};
	}
};
