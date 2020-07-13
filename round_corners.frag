#version 330 core

in vec2 gl_FragCoord;
out vec4 gl_FragColor;

float roundedBoxSDF(vec2 centerPos, vec2 size, vec4 radius) {
  radius.xy = (centerPos.x > 0.0) ? radius.xy : radius.zw;
  radius.x  = (centerPos.y > 0.0) ? radius.x  : radius.y;

  vec2 q = abs(centerPos) - size + radius.x;
  return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - radius.x;
}

void main()
{
  // The pixel space scale of the rectangle
  vec2 size = vec2(100.5, 60.5);
  
  // The pixel space location of the rectangle
  vec2 location = vec2(400.0, 100.0);

  // How soft edges should be (in pixels). Higher values could be used
  // to simulate a drop shadow.
  float edgeSoftness = 1.0;

  // The radius of the corners (in pixels) clockwise starting in the
  // top left
  vec4 radius = vec4(10.5, 10.5, 10.5, 10.5);

  // Calculate distance to edge
  float distance = roundedBoxSDF(gl_FragCoord.xy - location, size / 2.0, radius);

  // Smooth the result (free antialiasing)
  float smoothedAlpha = 1.0 - smoothstep(0.0, edgeSoftness, distance);

  // Colors
  vec4 bgColor = vec4(0.9, 0.9, 0.9, 1.0);
  vec4 fgColor = vec4(1.0, 0.0, 0.0, 1.0);

  gl_FragColor = mix(bgColor, fgColor, smoothedAlpha);
}
