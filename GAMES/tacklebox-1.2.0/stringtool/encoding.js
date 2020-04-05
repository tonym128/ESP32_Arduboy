/**
 * Tackle Box uses every letter and can draw any arbitary number, so just have a hardcoded
 * pretty standard encoding. This makes fishtool generating its strings much easier.
 *
 * TODO: wonder if would be possible to just do `renderer.drawString(TACKLE_BOX_STRING("goldfish"))`
 * where TACKLE_BOX_STRING is a constexpr that converts the string to this encoding, eliminating needing
 * stringtool at all
 */
module.exports = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*./,!\n"
    .split("")
    .reduce((encoding, ch, index) => {
        encoding[ch] = index;
        return encoding;
    }, {});
