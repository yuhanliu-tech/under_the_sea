@group(0) @binding(0) var<uniform> world_position: vec2f;
@group(0) @binding(1) var displacementMap: texture_storage_2d<r32float, write>;
@group(0) @binding(2) var normalMap: texture_storage_2d<rgba8unorm, write>;

const u_wind = vec2<f32>(1, 0);
const u_amplitude = f32(20.0);
const u_g = f32(9.81);
const PI = 3.14159265358979323846264; // Life of π
const l = 100.0;

fn random2(p: vec2<f32>) -> vec2<f32> {
    return fract(sin(vec2(dot(p, vec2(127.1f, 311.7f)),
                 dot(p, vec2(269.5f,183.3f))))
                 * 43758.5453f);
}

fn surflet(P: vec2<f32>, gridPoint: vec2<f32>) -> f32 {
    // Compute falloff function by converting linear distance to a polynomial
    let distX = f32(abs(P.x - gridPoint.x));
    let distY = f32(abs(P.y - gridPoint.y));
    let tX = f32(f32(1) - f32(6) * pow(distX, f32(5)) + f32(15) * pow(distX, f32(4)) - f32(10) * pow(distX, f32(3)));
    let tY = f32(f32(1) - f32(6) * pow(distY, f32(5)) + f32(15) * pow(distY, f32(4)) - f32(10) * pow(distY, f32(3)));
    
    // Get the random vector for the grid point
    let gradient = vec2<f32>(normalize(f32(2) * random2(gridPoint) - vec2<f32>(f32(1), f32(1))));
    // Get the vector from the grid point to P
    let diff = vec2<f32>(P - gridPoint);
    // Get the value of our height field by dotting grid->P with our gradient
    let height = f32(dot(diff, gradient));
    // Scale our height field (i.e. reduce it) by our polynomial falloff function
    return height * tX * tY;
}

fn perlinNoise(uv: vec2<f32>) -> f32 {
    var surfletSum = f32(0);
    // Iterate over the four integer corners surrounding uv
    for(var dx = 0; dx <= 1; dx++) {
            for(var dy = 0; dy <= 1; dy++) {
                    surfletSum += surflet(uv, floor(uv) + vec2(f32(dx), f32(dy)));
            }
    }
    return surfletSum;
}

@compute
@workgroup_size(16, 16)
fn main(@builtin(global_invocation_id) globalIdx: vec3u) {

    let x = f32(globalIdx.x);
    let y = f32(globalIdx.y);

    // Ocean settings
    let wave_amplitude = 1.0;        // Amplitude of the waves
    let wave_frequency = 0.1;        // Frequency of the waves
    let wave_direction = normalize(vec2f(1.0, 0.5)); // Direction of the waves
    let phase_shift = 0.0;  

    // Calculate the wave phase
    let position = vec2f(x, y);
    let wave_phase = wave_frequency * dot(position, wave_direction) + phase_shift;

     // Compute Gerstner wave height and horizontal displacement
    let wave_height = wave_amplitude * sin(wave_phase) * 0.5;
    let horizontal_displacement = wave_amplitude * cos(wave_phase) * wave_direction;

    let uv = vec2<f32>(x, y) + world_position + horizontal_displacement;
    let noise = wave_height * perlinNoise(uv / 10.0f);

    textureStore(displacementMap, globalIdx.xy, vec4(noise, 0, 0, 0));

    // Calculate normals using partial derivatives based on displaced positions
    let dx = wave_amplitude * -wave_frequency * cos(wave_phase) * wave_direction.x;
    let dy = wave_amplitude * -wave_frequency * cos(wave_phase) * wave_direction.y;
    let normal = normalize(vec3f(-dx, -dy, 1.0)) * perlinNoise(uv / 10.0f) * wave_height;
    
    // Store the computed normal in the normal map
    textureStore(normalMap, globalIdx.xy, vec4f(normal * 0.5 + 0.5, 1.0));  // Map from [-1, 1] to [0, 1]

}