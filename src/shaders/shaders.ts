// CHECKITOUT: this file loads all the shaders and preprocesses them with some common code

import commonRaw from './common.wgsl?raw';

import naiveVertRaw from './naive.vs.wgsl?raw';
import naiveFragRaw from './naive.fs.wgsl?raw';

import forwardPlusFragRaw from './forward_plus.fs.wgsl?raw';

import clusteredDeferredFragRaw from './clustered_deferred.fs.wgsl?raw';
import clusteredDeferredFullscreenVertRaw from './clustered_deferred_fullscreen.vs.wgsl?raw';
import clusteredDeferredFullscreenFragRaw from './clustered_deferred_fullscreen.fs.wgsl?raw';

import clusteringComputeRaw from './clustering.cs.wgsl?raw';

import oceanSurfaceVertRaw from './ocean_surface.vs.wgsl?raw';
import oceanSurfaceFragRaw from './ocean_surface.fs.wgsl?raw';
import oceanSurfaceComputeRaw from './ocean_surface.cs.wgsl?raw';

import outputTextureVertRaw from './output_texture.vs.wgsl?raw';
import outputTextureFragRaw from './output_texture.fs.wgsl?raw';

// CONSTANTS (for use in shaders)
// =================================

// CHECKITOUT: feel free to add more constants here and to refer to them in your shader code

// Note that these are declared in a somewhat roundabout way because otherwise minification will drop variables
// that are unused in host side code.
export const constants = {
    bindGroup_scene: 0,
    bindGroup_fullscreen: 1,
    bindGroup_model: 1,
    bindGroup_material: 2,

    moveLightsWorkgroupSize: 128,

    lightRadius: 2,
    clusterX: 16,
    clusterY: 16,
    clusterZ: 16,

    maxClusterLights: 1024,
};

// =================================

function evalShaderRaw(raw: string) {
    return eval('`' + raw.replaceAll('${', '${constants.') + '`');
}

const commonSrc: string = evalShaderRaw(commonRaw);

function processShaderRaw(raw: string) {
    return commonSrc + evalShaderRaw(raw);
}

export const naiveVertSrc: string = processShaderRaw(naiveVertRaw);
export const naiveFragSrc: string = processShaderRaw(naiveFragRaw);

export const forwardPlusFragSrc: string = processShaderRaw(forwardPlusFragRaw);

export const clusteredDeferredFragSrc: string = processShaderRaw(clusteredDeferredFragRaw);
export const clusteredDeferredFullscreenVertSrc: string = processShaderRaw(clusteredDeferredFullscreenVertRaw);
export const clusteredDeferredFullscreenFragSrc: string = processShaderRaw(clusteredDeferredFullscreenFragRaw);

export const clusteringComputeSrc: string = processShaderRaw(clusteringComputeRaw);

export const oceanSurfaceVertSrc: string = processShaderRaw(oceanSurfaceVertRaw);
export const oceanSurfaceFragSrc: string = processShaderRaw(oceanSurfaceFragRaw);
export const oceanSurfaceComputeSrc: string = processShaderRaw(oceanSurfaceComputeRaw);

export const outputTextureFragSrc: string = processShaderRaw(outputTextureFragRaw);
export const outputTextureVertSrc: string = processShaderRaw(outputTextureVertRaw);

