// WebGPU type definitions
// This file provides basic WebGPU types until they're officially available in TypeScript

interface Navigator {
    gpu?: GPU;
}

interface GPU {
    requestAdapter(options?: GPURequestAdapterOptions): Promise<GPUAdapter | null>;
    getPreferredCanvasFormat(): GPUTextureFormat;
}

interface GPURequestAdapterOptions {
    powerPreference?: 'low-power' | 'high-performance';
    forceFallbackAdapter?: boolean;
}

interface GPUAdapter {
    info: GPUAdapterInfo;
    requestDevice(descriptor?: GPUDeviceDescriptor): Promise<GPUDevice>;
}

interface GPUAdapterInfo {
    vendor: string;
    architecture: string;
    device: string;
    description: string;
}

interface GPUDeviceDescriptor {
    label?: string;
    requiredFeatures?: GPUFeatureName[];
    requiredLimits?: Record<string, number>;
}

type GPUFeatureName = string;

interface GPUDevice {
    label: string;
    queue: GPUQueue;
    createCommandEncoder(descriptor?: GPUCommandEncoderDescriptor): GPUCommandEncoder;
    createShaderModule(descriptor: GPUShaderModuleDescriptor): GPUShaderModule;
    createRenderPipeline(descriptor: GPURenderPipelineDescriptor): GPURenderPipeline;
    destroy(): void;
}

interface GPUQueue {
    submit(commandBuffers: GPUCommandBuffer[]): void;
}

interface GPUCommandEncoderDescriptor {
    label?: string;
}

interface GPUCommandEncoder {
    beginRenderPass(descriptor: GPURenderPassDescriptor): GPURenderPassEncoder;
    finish(): GPUCommandBuffer;
}

interface GPUCommandBuffer {}

interface GPUShaderModuleDescriptor {
    code: string;
    label?: string;
}

interface GPUShaderModule {}

interface GPURenderPipelineDescriptor {
    layout: 'auto' | GPUPipelineLayout;
    vertex: GPUVertexState;
    fragment?: GPUFragmentState;
    primitive?: GPUPrimitiveState;
    label?: string;
}

interface GPUPipelineLayout {}

interface GPUVertexState {
    module: GPUShaderModule;
    entryPoint: string;
    buffers?: GPUVertexBufferLayout[];
}

interface GPUVertexBufferLayout {}

interface GPUFragmentState {
    module: GPUShaderModule;
    entryPoint: string;
    targets: GPUColorTargetState[];
}

interface GPUColorTargetState {
    format: GPUTextureFormat;
    blend?: GPUBlendState;
    writeMask?: number;
}

interface GPUBlendState {}

interface GPUPrimitiveState {
    topology?: GPUPrimitiveTopology;
    stripIndexFormat?: GPUIndexFormat;
    frontFace?: GPUFrontFace;
    cullMode?: GPUCullMode;
}

type GPUPrimitiveTopology = 'point-list' | 'line-list' | 'line-strip' | 'triangle-list' | 'triangle-strip';
type GPUIndexFormat = 'uint16' | 'uint32';
type GPUFrontFace = 'ccw' | 'cw';
type GPUCullMode = 'none' | 'front' | 'back';

interface GPURenderPipeline {}

interface GPURenderPassDescriptor {
    colorAttachments: (GPURenderPassColorAttachment | null)[];
    depthStencilAttachment?: GPURenderPassDepthStencilAttachment;
    label?: string;
}

interface GPURenderPassColorAttachment {
    view: GPUTextureView;
    resolveTarget?: GPUTextureView;
    clearValue?: GPUColor;
    loadOp: GPULoadOp;
    storeOp: GPUStoreOp;
}

interface GPURenderPassDepthStencilAttachment {}

interface GPUTextureView {}

interface GPUColor {
    r: number;
    g: number;
    b: number;
    a: number;
}

type GPULoadOp = 'load' | 'clear';
type GPUStoreOp = 'store' | 'discard';

interface GPURenderPassEncoder {
    setPipeline(pipeline: GPURenderPipeline): void;
    draw(vertexCount: number, instanceCount?: number, firstVertex?: number, firstInstance?: number): void;
    end(): void;
}

interface GPUCanvasContext {
    configure(configuration: GPUCanvasConfiguration): void;
    getCurrentTexture(): GPUTexture;
}

interface GPUCanvasConfiguration {
    device: GPUDevice;
    format: GPUTextureFormat;
    usage?: number;
    alphaMode?: GPUCanvasAlphaMode;
}

type GPUCanvasAlphaMode = 'opaque' | 'premultiplied';

interface GPUTexture {
    createView(descriptor?: GPUTextureViewDescriptor): GPUTextureView;
}

interface GPUTextureViewDescriptor {}

type GPUTextureFormat = 
    | 'r8unorm'
    | 'r8snorm'
    | 'r8uint'
    | 'r8sint'
    | 'r16uint'
    | 'r16sint'
    | 'r16float'
    | 'rg8unorm'
    | 'rg8snorm'
    | 'rg8uint'
    | 'rg8sint'
    | 'r32uint'
    | 'r32sint'
    | 'r32float'
    | 'rg16uint'
    | 'rg16sint'
    | 'rg16float'
    | 'rgba8unorm'
    | 'rgba8unorm-srgb'
    | 'rgba8snorm'
    | 'rgba8uint'
    | 'rgba8sint'
    | 'bgra8unorm'
    | 'bgra8unorm-srgb'
    | 'rgb10a2unorm'
    | 'rg11b10ufloat'
    | 'rg32uint'
    | 'rg32sint'
    | 'rg32float'
    | 'rgba16uint'
    | 'rgba16sint'
    | 'rgba16float'
    | 'rgba32uint'
    | 'rgba32sint'
    | 'rgba32float';

interface HTMLCanvasElement {
    getContext(contextId: 'webgpu'): GPUCanvasContext | null;
}