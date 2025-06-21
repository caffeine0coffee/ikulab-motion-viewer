import React from 'react';
import { Play, Pause, Info, FileVideo, PlayCircle, Download } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Badge } from '@/components/ui/badge';
import type { RendererInfo } from './WebGPUCanvas';

interface ControlPanelProps {
  webgpuStatus: 'loading' | 'success' | 'error';
  webgpuError?: string;
  rendererInfo?: RendererInfo;
  onToggleRender: () => void;
  isRenderingDisabled: boolean;
}

export const ControlPanel: React.FC<ControlPanelProps> = ({
  webgpuStatus,
  webgpuError,
  rendererInfo,
  onToggleRender,
  isRenderingDisabled
}) => {
  const getWebGPUStatusDisplay = () => {
    switch (webgpuStatus) {
      case 'loading':
        return (
          <Badge variant="outline" className="bg-blue-50 text-blue-700 border-blue-200">
            <Info className="w-3 h-3 mr-1" />
            Initializing...
          </Badge>
        );
      case 'success':
        return (
          <Badge variant="default" className="bg-green-100 text-green-800 border-green-200">
            <span className="w-2 h-2 bg-green-500 rounded-full mr-2"></span>
            WebGPU Ready
          </Badge>
        );
      case 'error':
        return (
          <Badge variant="destructive">
            <span className="w-2 h-2 bg-red-500 rounded-full mr-2"></span>
            WebGPU Error
          </Badge>
        );
      default:
        return (
          <Badge variant="outline">
            Unknown Status
          </Badge>
        );
    }
  };

  const getWebGPUFallbackMessage = () => {
    if (webgpuStatus !== 'error') return null;

    return (
      <div className="mt-4 p-4 bg-red-50 border border-red-200 rounded-md">
        <p className="text-sm font-medium text-red-800 mb-2">WebGPU is not available</p>
        <p className="text-xs text-red-600 mb-2">
          Please use a browser that supports WebGPU:
        </p>
        <ul className="text-xs text-red-600 space-y-1">
          <li>• Chrome 113+ (enable WebGPU flag)</li>
          <li>• Edge 113+ (enable WebGPU flag)</li>
          <li>• Firefox Nightly (enable WebGPU flag)</li>
        </ul>
        {webgpuError && (
          <p className="text-xs text-red-700 mt-2 font-medium">
            Error: {webgpuError}
          </p>
        )}
      </div>
    );
  };

  return (
    <div className="w-80 bg-white border-l border-gray-200 p-6 overflow-y-auto">
      <div className="space-y-6">
        
        {/* WebGPU Status */}
        <Card>
          <CardHeader className="pb-3">
            <CardTitle className="text-sm font-medium">WebGPU Status</CardTitle>
          </CardHeader>
          <CardContent className="pt-0">
            {getWebGPUStatusDisplay()}
            {getWebGPUFallbackMessage()}
          </CardContent>
        </Card>

        {/* Render Controls */}
        <Card>
          <CardHeader className="pb-3">
            <CardTitle className="text-sm font-medium">Render Controls</CardTitle>
          </CardHeader>
          <CardContent className="pt-0">
            <Button
              size="lg"
              onClick={onToggleRender}
              disabled={isRenderingDisabled}
              className="w-full"
            >
              {rendererInfo?.isRendering ? (
                <>
                  <Pause className="w-4 h-4 mr-2" />
                  Stop Rendering
                </>
              ) : (
                <>
                  <Play className="w-4 h-4 mr-2" />
                  Start Rendering
                </>
              )}
            </Button>
          </CardContent>
        </Card>

        {/* Debug Info */}
        <Card>
          <CardHeader className="pb-3">
            <CardTitle className="text-sm font-medium">Debug Info</CardTitle>
          </CardHeader>
          <CardContent className="pt-0">
            {rendererInfo ? (
              <div className="debug-info">
                <div className="debug-info-item">
                  <span className="debug-info-label">Canvas:</span>
                  <span className="debug-info-value">{rendererInfo.canvasSize}</span>
                </div>
                <div className="debug-info-item">
                  <span className="debug-info-label">Device Ratio:</span>
                  <span className="debug-info-value">{rendererInfo.deviceRatio}</span>
                </div>
                <div className="debug-info-item">
                  <span className="debug-info-label">Adapter:</span>
                  <span className="debug-info-value">{rendererInfo.adapter}</span>
                </div>
                <div className="debug-info-item">
                  <span className="debug-info-label">Device:</span>
                  <span className="debug-info-value">{rendererInfo.device}</span>
                </div>
                <div className="debug-info-item">
                  <span className="debug-info-label">FPS:</span>
                  <span className="debug-info-value">{rendererInfo.fps}</span>
                </div>
                <div className="debug-info-item">
                  <span className="debug-info-label">Status:</span>
                  <span className="debug-info-value">
                    {rendererInfo.isRendering ? 'Rendering' : 'Stopped'}
                  </span>
                </div>
              </div>
            ) : (
              <p className="text-sm text-gray-500">No debug information available</p>
            )}
          </CardContent>
        </Card>

        {/* Future Controls */}
        <Card className="opacity-60">
          <CardHeader className="pb-3">
            <CardTitle className="text-sm font-medium">Motion Controls</CardTitle>
          </CardHeader>
          <CardContent className="pt-0 space-y-3">
            <Button variant="outline" disabled className="w-full justify-start">
              <FileVideo className="w-4 h-4 mr-2" />
              Load BVH File
            </Button>
            <Button variant="outline" disabled className="w-full justify-start">
              <PlayCircle className="w-4 h-4 mr-2" />
              Play Animation
            </Button>
            <Button variant="outline" disabled className="w-full justify-start">
              <Download className="w-4 h-4 mr-2" />
              Export Animation
            </Button>
            <div className="pt-2 border-t border-gray-100">
              <p className="text-xs text-gray-500">
                Coming soon in next iterations
              </p>
            </div>
          </CardContent>
        </Card>

      </div>
    </div>
  );
};

export default ControlPanel;