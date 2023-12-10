$input v_pos, v_view, v_normal, v_color0, v_texcoord0
#include "common.sh"

SAMPLER2D(s_texColor, 0);

void main()
{
	gl_FragColor = texture2D(s_texColor, v_texcoord0);
}
