export class WebGPURenderer {
    private canvas: HTMLCanvasElement;
    private context: GPUCanvasContext | null = null;
    private device: GPUDevice | null = null;
    private adapter: GPUAdapter | null = null;
    private renderPipeline: GPURenderPipeline | null = null;
    private isInitialized = false;
    
    constructor(canvas: HTMLCanvasElement) {
        this.canvas = canvas;
    }
    
    async initialize(): Promise<boolean> {
        try {
            console.log('WebGPURenderer: Starting initialization');
            
            // Check WebGPU support
            if (!navigator.gpu) {
                throw new Error('WebGPU is not supported in this browser');
            }
            console.log('WebGPU is supported');
            
            // Get adapter
            this.adapter = await navigator.gpu.requestAdapter();
            if (!this.adapter) {
                throw new Error('No WebGPU adapter found');
            }
            console.log('WebGPU adapter obtained:', this.adapter);
            
            // Get device
            this.device = await this.adapter.requestDevice();
            if (!this.device) {
                throw new Error('Failed to get WebGPU device');
            }
            console.log('WebGPU device obtained:', this.device);
            
            // Configure canvas context
            this.context = this.canvas.getContext('webgpu');
            if (!this.context) {
                throw new Error('Failed to get WebGPU context');
            }
            console.log('WebGPU context obtained');
            
            const canvasFormat = navigator.gpu.getPreferredCanvasFormat();
            console.log('Canvas format:', canvasFormat);
            
            this.context.configure({
                device: this.device,
                format: canvasFormat,
                alphaMode: 'premultiplied'
            });
            console.log('Canvas context configured');
            
            // Create render pipeline for triangle
            await this.createTriangleRenderPipeline(canvasFormat);
            console.log('Render pipeline created');
            
            this.isInitialized = true;
            console.log('WebGPURenderer: Initialization complete');
            return true;
            
        } catch (error) {
            console.error('Failed to initialize WebGPU:', error);
            return false;
        }
    }
    
    private async createTriangleRenderPipeline(format: GPUTextureFormat): Promise<void> {
        if (!this.device) return;
        
        const vertexShaderCode = `
            struct VertexOutput {
                @builtin(position) position: vec4<f32>,
                @location(0) color: vec3<f32>,
            }
            
            @vertex
            fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
                var pos = array<vec2<f32>, 3>(
                    vec2<f32>( 0.0,  0.5),
                    vec2<f32>(-0.5, -0.5),
                    vec2<f32>( 0.5, -0.5)
                );
                
                var color = array<vec3<f32>, 3>(
                    vec3<f32>(1.0, 0.0, 0.0), // Red
                    vec3<f32>(0.0, 1.0, 0.0), // Green
                    vec3<f32>(0.0, 0.0, 1.0)  // Blue
                );
                
                var output: VertexOutput;
                output.position = vec4<f32>(pos[vertexIndex], 0.0, 1.0);
                output.color = color[vertexIndex];
                return output;
            }
        `;
        
        const fragmentShaderCode = `
            @fragment
            fn fs_main(@location(0) color: vec3<f32>) -> @location(0) vec4<f32> {
                return vec4<f32>(color, 1.0);
            }
        `;
        
        const vertexShader = this.device.createShaderModule({
            code: vertexShaderCode
        });
        
        const fragmentShader = this.device.createShaderModule({
            code: fragmentShaderCode
        });
        
        this.renderPipeline = this.device.createRenderPipeline({
            layout: 'auto',
            vertex: {
                module: vertexShader,
                entryPoint: 'vs_main'
            },
            fragment: {
                module: fragmentShader,
                entryPoint: 'fs_main',
                targets: [{
                    format: format
                }]
            },
            primitive: {
                topology: 'triangle-list'
            }
        });
    }
    
    render(): void {
        if (!this.isInitialized || !this.device || !this.context || !this.renderPipeline) {
            console.log('Render called but not ready:', {
                isInitialized: this.isInitialized,
                hasDevice: !!this.device,
                hasContext: !!this.context,
                hasPipeline: !!this.renderPipeline
            });
            return;
        }
        
        try {
            const commandEncoder = this.device.createCommandEncoder();
            const textureView = this.context.getCurrentTexture().createView();
            
            const renderPassDescriptor: GPURenderPassDescriptor = {
                colorAttachments: [{
                    view: textureView,
                    clearValue: { r: 0.1, g: 0.1, b: 0.1, a: 1.0 },
                    loadOp: 'clear',
                    storeOp: 'store'
                }]
            };
            
            const passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
            passEncoder.setPipeline(this.renderPipeline);
            passEncoder.draw(3); // Draw triangle (3 vertices)
            passEncoder.end();
            
            this.device.queue.submit([commandEncoder.finish()]);
        } catch (error) {
            console.error('Render error:', error);
        }
    }
    
    resize(width: number, height: number): void {
        this.canvas.width = width;
        this.canvas.height = height;
    }
    
    getInfo(): { adapter: string; device: string } {
        const adapterInfo = this.adapter?.info;
        return {
            adapter: adapterInfo ? `${adapterInfo.vendor} ${adapterInfo.architecture}` : 'Unknown',
            device: this.device?.label || 'Unknown'
        };
    }
    
    destroy(): void {
        if (this.device) {
            this.device.destroy();
        }
        this.isInitialized = false;
    }
}