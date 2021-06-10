struct Simple
{
	float x = 0, y = 0;

	void sketch(uint8_t const *sprite, uint8_t const &frame = 0) const
	{
		sprites.drawSelfMasked((int16_t) (x - camera.x), (int16_t) (y - camera.y), sprite, frame);
	}
};
