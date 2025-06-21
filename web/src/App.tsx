import React, { useState, useCallback } from 'react';
import WebGPUCanvas, { type RendererInfo } from './components/WebGPUCanvas';
import ControlPanel from './components/ControlPanel';
import StatusOverlay from './components/StatusOverlay';

type WebGPUStatus = 'loading' | 'success' | 'error';

interface StatusState {
  message: string;
  type: 'info' | 'success' | 'error';
}

const App: React.FC = () => {
  const [webgpuStatus, setWebGpuStatus] = useState<WebGPUStatus>('loading');
  const [webgpuError, setWebGpuError] = useState<string>();
  const [rendererInfo, setRendererInfo] = useState<RendererInfo>();
  const [status, setStatus] = useState<StatusState>({ message: 'Initializing...', type: 'info' });
  const [isRendering, setIsRendering] = useState(false);

  const handleStatusChange = useCallback((message: string, type: 'info' | 'success' | 'error') => {
    setStatus({ message, type });
    
    // Update WebGPU status based on message content
    if (message.includes('WebGPU initialized successfully')) {
      setWebGpuStatus('success');
      setWebGpuError(undefined);
    } else if (message.includes('Failed to initialize WebGPU') || message.includes('Error:')) {
      setWebGpuStatus('error');
      setWebGpuError(message);
    }
  }, []);

  const handleInfoUpdate = useCallback((info: RendererInfo) => {
    setRendererInfo(info);
    setIsRendering(info.isRendering);
  }, []);

  const handleToggleRender = useCallback(() => {
    setIsRendering(prev => !prev);
  }, []);

  const isRenderingDisabled = webgpuStatus !== 'success';

  return (
    <div className="h-screen bg-gray-50 fade-in">
      {/* Header */}
      <header className="bg-gray-900 text-white px-6 py-4 shadow-sm">
        <h1 className="text-lg font-semibold">
          ikulab Motion Viewer - Web Edition
        </h1>
      </header>
      
      {/* Main Layout */}
      <div className="flex h-[calc(100vh-4rem)]">
        {/* Canvas Area */}
        <main className="flex-1 p-4">
          <div className="relative h-full">
            <WebGPUCanvas 
              onStatusChange={handleStatusChange}
              onInfoUpdate={handleInfoUpdate}
              shouldRender={isRendering}
            />
            <StatusOverlay 
              message={status.message}
              type={status.type}
            />
          </div>
        </main>
        
        {/* Control Panel */}
        <aside>
          <ControlPanel
            webgpuStatus={webgpuStatus}
            webgpuError={webgpuError}
            rendererInfo={rendererInfo}
            onToggleRender={handleToggleRender}
            isRenderingDisabled={isRenderingDisabled}
          />
        </aside>
      </div>
    </div>
  );
};

export default App;