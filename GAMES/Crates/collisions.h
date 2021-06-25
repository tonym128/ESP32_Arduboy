// copied from the arduboy2 library and modified to use references
bool collide(Rect const &rect1, Rect const &rect2)
{
	return !(rect2.x                >= rect1.x + rect1.width  ||
	         rect2.x + rect2.width  <= rect1.x                ||
	         rect2.y                >= rect1.y + rect1.height ||
	         rect2.y + rect2.height <= rect1.y);
}

void separate(Solid &first, Solid const &second)
{
	float translateX = 0;
	float translateY = 0;

	// find x translation distance
	if (first.cbox.x < second.cbox.x)
	{
		translateX = (second.cbox.x - first.cbox_conf.width - first.cbox_conf.x + 1) - first.x;
	}
	else
	{
		translateX = (second.cbox.x + second.cbox.width - first.cbox_conf.x) - first.x;
	}

	// find y translation distance
	if (first.cbox.y < second.cbox.y)
	{
		translateY = (second.cbox.y - first.cbox_conf.height - first.cbox_conf.y + 1) - first.y;
	}
	else
	{
		translateY = (second.cbox.y + second.cbox.height - first.cbox_conf.y) - first.y;
	}

	// apply the translation
	if (abs(translateX) < abs(translateY))
	{
		first.x += translateX;
	}
	else
	{
		first.y += translateY;
	}
}
