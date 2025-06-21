import React, { useRef, useEffect, useState, useCallback } from 'react';
import { WebGPURenderer } from '../webgpu-renderer';

interface WebGPUCanvasProps {
  onStatusChange?: (status: string, type: 'info' | 'success' | 'error') => void;
  onInfoUpdate?: (info: RendererInfo) => void;
  shouldRender?: boolean;
}

export interface RendererInfo {
  adapter: string;
  device: string;
  canvasSize: string;
  deviceRatio: number;
  fps: string;
  isRendering: boolean;
}

export const WebGPUCanvas: React.FC<WebGPUCanvasProps> = ({
  onStatusChange,
  onInfoUpdate,
  shouldRender = false
}) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const rendererRef = useRef<WebGPURenderer | null>(null);
  const containerRef = useRef<HTMLDivElement>(null);
  const animationIdRef = useRef<number | null>(null);
  
  const [isInitialized, setIsInitialized] = useState(false);
  const [isRendering, setIsRendering] = useState(false);

  const updateInfo = useCallback(() => {
    if (rendererRef.current && canvasRef.current && onInfoUpdate) {
      const info = rendererRef.current.getInfo();
      onInfoUpdate({
        adapter: info.adapter,
        device: info.device,
        canvasSize: `${canvasRef.current.width}×${canvasRef.current.height}`,
        deviceRatio: window.devicePixelRatio,
        fps: isRendering ? '~60' : '0',
        isRendering
      });
    }
  }, [isRendering, onInfoUpdate]);

  const resizeCanvas = useCallback(() => {
    if (!canvasRef.current || !containerRef.current || !rendererRef.current) return;

    const container = containerRef.current;
    const rect = container.getBoundingClientRect();

    // Set canvas size to match container
    canvasRef.current.width = rect.width * window.devicePixelRatio;
    canvasRef.current.height = rect.height * window.devicePixelRatio;
    canvasRef.current.style.width = rect.width + 'px';
    canvasRef.current.style.height = rect.height + 'px';

    rendererRef.current.resize(canvasRef.current.width, canvasRef.current.height);
    updateInfo();
  }, [updateInfo]);

  const initialize = useCallback(async () => {
    if (!canvasRef.current) {
      console.log('Canvas ref is null during initialization');
      return;
    }

    console.log('Initializing WebGPU with canvas:', canvasRef.current);

    try {
      onStatusChange?.('Initializing WebGPU...', 'info');
      
      const renderer = new WebGPURenderer(canvasRef.current);
      const success = await renderer.initialize();

      console.log('WebGPU initialization result:', success);

      if (success) {
        rendererRef.current = renderer;
        setIsInitialized(true);
        onStatusChange?.('WebGPU initialized successfully', 'success');
        
        // Ensure canvas is properly sized before first render
        setTimeout(() => {
          resizeCanvas();
          updateInfo();
        }, 100);
      } else {
        onStatusChange?.('Failed to initialize WebGPU', 'error');
      }
    } catch (error) {
      console.error('WebGPU initialization error:', error);
      onStatusChange?.(`Error: ${error}`, 'error');
    }
  }, [onStatusChange, resizeCanvas, updateInfo]);

  const startRendering = useCallback(() => {
    if (!rendererRef.current) {
      console.log('Cannot start rendering: no renderer');
      return;
    }

    if (isRendering) {
      console.log('Already rendering, skipping start');
      return;
    }

    console.log('Starting rendering...');
    setIsRendering(true);
    onStatusChange?.('Rendering started', 'success');

    let renderingActive = true;

    const render = () => {
      if (!renderingActive || !rendererRef.current) {
        console.log('Stopping render loop:', { renderingActive, hasRenderer: !!rendererRef.current });
        return;
      }

      try {
        rendererRef.current.render();
        updateInfo();
        animationIdRef.current = requestAnimationFrame(render);
      } catch (error) {
        console.error('Render error:', error);
        renderingActive = false;
        setIsRendering(false);
        onStatusChange?.(`Render error: ${error}`, 'error');
      }
    };

    // Store reference to control render loop
    animationIdRef.current = requestAnimationFrame(render);
    
    // Return cleanup function to stop rendering
    return () => {
      renderingActive = false;
    };
  }, [onStatusChange, updateInfo]);

  const stopRendering = useCallback(() => {
    console.log('Stopping rendering...');
    setIsRendering(false);
    onStatusChange?.('Rendering stopped', 'info');

    if (animationIdRef.current !== null) {
      cancelAnimationFrame(animationIdRef.current);
      animationIdRef.current = null;
    }

    updateInfo();
  }, [onStatusChange, updateInfo]);

  // Initialize WebGPU when component mounts
  useEffect(() => {
    initialize();
  }, [initialize]);

  // Handle window resize
  useEffect(() => {
    const handleResize = () => resizeCanvas();
    
    window.addEventListener('resize', handleResize);
    
    // Setup ResizeObserver for container
    const resizeObserver = new ResizeObserver(handleResize);
    if (containerRef.current) {
      resizeObserver.observe(containerRef.current);
    }

    return () => {
      window.removeEventListener('resize', handleResize);
      resizeObserver.disconnect();
    };
  }, [resizeCanvas]);

  // Cleanup on unmount
  useEffect(() => {
    return () => {
      if (animationIdRef.current !== null) {
        cancelAnimationFrame(animationIdRef.current);
      }
      if (rendererRef.current) {
        rendererRef.current.destroy();
      }
    };
  }, []);

  // Handle shouldRender prop changes
  useEffect(() => {
    console.log('shouldRender effect:', { shouldRender, isInitialized, isRendering });
    
    if (!isInitialized) {
      console.log('Not initialized yet, skipping render control');
      return;
    }

    if (shouldRender && !isRendering) {
      console.log('Should start rendering');
      startRendering();
    } else if (!shouldRender && isRendering) {
      console.log('Should stop rendering');
      stopRendering();
    }
  }, [shouldRender, isInitialized, isRendering, startRendering, stopRendering]);

  return (
    <div ref={containerRef} className="canvas-container">
      <canvas
        ref={canvasRef}
        className="webgpu-canvas"
      />
    </div>
  );
};

export default WebGPUCanvas;