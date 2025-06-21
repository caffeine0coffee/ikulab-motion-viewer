import React, { useState, useEffect } from 'react';

interface StatusOverlayProps {
  message: string;
  type: 'info' | 'success' | 'error';
  duration?: number; // Auto-hide duration in ms, 0 = no auto-hide
}

export const StatusOverlay: React.FC<StatusOverlayProps> = ({
  message,
  type,
  duration = 3000
}) => {
  const [visible, setVisible] = useState(true);
  const [opacity, setOpacity] = useState(1);

  useEffect(() => {
    setVisible(true);
    setOpacity(1);

    if (duration > 0 && type === 'success') {
      const timer = setTimeout(() => {
        setOpacity(0);
        setTimeout(() => {
          setVisible(false);
        }, 300);
      }, duration);

      return () => clearTimeout(timer);
    }
  }, [message, type, duration]);

  if (!visible || !message) {
    return null;
  }

  return (
    <div 
      className={`status-overlay ${type}`}
      style={{ opacity }}
    >
      {message}
    </div>
  );
};

export default StatusOverlay;