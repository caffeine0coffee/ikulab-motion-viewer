# 開発ガイド

ikulab Motion Viewer Web Editionの開発に関する詳細な情報をまとめています。

## 📋 プロジェクト構造

### ディレクトリ構成
```
web/
├── public/                    # 静的アセット
├── src/                       # ソースコード
│   ├── components/           # Reactコンポーネント
│   │   ├── ui/              # shadcn/ui基本コンポーネント
│   │   ├── WebGPUCanvas.tsx # WebGPUレンダリング
│   │   ├── ControlPanel.tsx # 制御パネル
│   │   └── StatusOverlay.tsx # ステータス表示
│   ├── lib/                 # ユーティリティライブラリ
│   ├── styles/              # スタイルファイル
│   ├── webgpu-renderer.ts   # WebGPUエンジン
│   ├── webgpu-types.d.ts    # WebGPU型定義
│   ├── App.tsx              # メインアプリ
│   └── main.tsx             # エントリーポイント
├── tailwind.config.js       # Tailwind CSS設定
├── tsconfig.json            # TypeScript設定
├── vite.config.ts           # Vite設定
├── package.json             # パッケージ定義
├── README.md                # プロジェクト概要
├── ARCHITECTURE.md          # アーキテクチャ詳細
└── DEVELOPMENT.md           # 開発ガイド（このファイル）
```

## 🔧 依存関係詳細

### プロダクション依存関係

#### コアライブラリ
```json
{
  "react": "^18.2.0",           // UIライブラリ
  "react-dom": "^18.2.0",       // DOM操作
  "gl-matrix": "^3.4.3"         // 3D数学演算
}
```

#### UIフレームワーク
```json
{
  "@radix-ui/react-slot": "^1.0.2",         // Radix UI基盤
  "class-variance-authority": "^0.7.0",     // CSS variant管理
  "clsx": "^2.0.0",                         // クラス名条件分岐
  "lucide-react": "^0.294.0",               // アイコンライブラリ
  "tailwind-merge": "^2.0.0"                // Tailwind クラス結合
}
```

### 開発依存関係

#### 型定義
```json
{
  "@types/react": "^18.2.0",        // React型定義
  "@types/react-dom": "^18.2.0",    // ReactDOM型定義
  "@types/gl-matrix": "^3.2.0"      // gl-matrix型定義
}
```

#### ビルドツール
```json
{
  "@vitejs/plugin-react": "^4.0.0", // Vite React プラグイン
  "typescript": "^5.0.0",           // TypeScript コンパイラ
  "vite": "^4.4.0"                  // ビルドツール
}
```

#### スタイリング
```json
{
  "tailwindcss": "^3.3.0",         // CSS フレームワーク
  "autoprefixer": "^10.4.16",      // CSS プレフィックス自動付与
  "postcss": "^8.4.31",            // CSS 変換ツール
  "tailwindcss-animate": "^1.0.7"  // Tailwind アニメーション
}
```

## 🚀 開発ワークフロー

### 初期セットアップ
```bash
# リポジトリクローン
git clone <repository-url>
cd ikulab-motion-viewer/web

# 依存関係インストール
npm install

# 開発サーバー起動
npm run dev
```

### 日常的な開発フロー
```bash
# 1. 機能ブランチ作成
git checkout -b feature/new-feature

# 2. 開発サーバー起動
npm run dev

# 3. コード変更・テスト

# 4. ビルドテスト
npm run build

# 5. 型チェック
npx tsc --noEmit

# 6. コミット
git add .
git commit -m "feat: 新機能の実装"

# 7. プッシュ
git push origin feature/new-feature
```

## 🛠 ビルドシステム詳細

### Vite設定 (`vite.config.ts`)
```typescript
export default defineConfig({
  plugins: [react()],              // React サポート
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src")  // @/ エイリアス
    }
  },
  server: {
    port: 3000,                    // 開発サーバーポート
    host: true                     // ネットワークアクセス許可
  },
  build: {
    target: 'esnext',              // モダンブラウザ向け
    outDir: 'dist'                 // 出力ディレクトリ
  },
  optimizeDeps: {
    include: ['gl-matrix', 'react', 'react-dom']  // 事前バンドル
  }
});
```

### TypeScript設定 (`tsconfig.json`)
```json
{
  "compilerOptions": {
    "target": "ES2020",                    // ターゲット仕様
    "lib": ["ES2020", "DOM", "DOM.Iterable"],
    "module": "ESNext",                    // モジュール仕様
    "moduleResolution": "bundler",         // Vite用設定
    "jsx": "react-jsx",                    // React JSX
    "strict": true,                        // 厳密な型チェック
    "baseUrl": ".",                        // パス解決基準
    "paths": {
      "@/*": ["./src/*"]                   // エイリアス設定
    }
  },
  "include": ["src/**/*", "src/**/*.d.ts"],
  "exclude": ["node_modules", "dist"]
}
```

### Tailwind設定 (`tailwind.config.js`)
```javascript
module.exports = {
  content: ["./src/**/*.{ts,tsx}"],        // 監視対象
  theme: {
    extend: {
      colors: {
        // shadcn/ui カラーシステム
        border: "hsl(var(--border))",
        background: "hsl(var(--background))",
        // ... その他カスタムカラー
      }
    }
  },
  plugins: [require("tailwindcss-animate")]  // アニメーション
};
```

## 🎯 開発タスク

### NPMスクリプト
```json
{
  "dev": "vite",                    // 開発サーバー起動
  "build": "tsc && vite build",     // プロダクションビルド
  "preview": "vite preview",        // ビルド結果プレビュー
  "serve": "vite preview --port 3000"  // ポート指定プレビュー
}
```

### 追加の開発用スクリプト（推奨）
```bash
# 型チェックのみ
npx tsc --noEmit

# Tailwind CSS 開発モード
npx tailwindcss -i ./src/styles/globals.css -o ./dist/output.css --watch

# 依存関係の脆弱性チェック
npm audit

# 依存関係の更新
npm update
```

## 🧪 コード品質管理

### TypeScript厳密設定
```json
{
  "strict": true,                         // すべての厳密チェック有効
  "noUnusedLocals": true,                // 未使用ローカル変数エラー
  "noUnusedParameters": true,            // 未使用パラメーターエラー
  "noFallthroughCasesInSwitch": true     // switch文の fallthrough エラー
}
```

### 推奨ESLint設定（将来実装）
```json
{
  "extends": [
    "@typescript-eslint/recommended",
    "plugin:react/recommended",
    "plugin:react-hooks/recommended"
  ],
  "rules": {
    "@typescript-eslint/no-unused-vars": "error",
    "react/prop-types": "off"
  }
}
```

### 推奨Prettier設定（将来実装）
```json
{
  "printWidth": 80,
  "tabWidth": 2,
  "useTabs": false,
  "semi": true,
  "singleQuote": true,
  "trailingComma": "es5"
}
```

## 🐛 デバッグ手法

### WebGPUデバッグ
```typescript
// WebGPU初期化デバッグ
console.log('WebGPU support:', navigator.gpu);
console.log('Adapter:', await navigator.gpu.requestAdapter());

// レンダリングパフォーマンス監視
const stats = {
  frameCount: 0,
  lastTime: performance.now()
};

function renderLoop() {
  stats.frameCount++;
  const now = performance.now();
  if (now - stats.lastTime >= 1000) {
    console.log(`FPS: ${stats.frameCount}`);
    stats.frameCount = 0;
    stats.lastTime = now;
  }
}
```

### React Developer Tools
```bash
# React DevTools拡張機能の使用推奨
# - Component tree inspection
# - Props/State monitoring
# - Performance profiling
```

### ブラウザDevTools活用
```javascript
// パフォーマンス測定
console.time('render');
renderer.render();
console.timeEnd('render');

// メモリ使用量監視
console.log('Memory:', performance.memory);

// WebGPU情報
console.log('Canvas:', canvas.getContext('webgpu'));
```

## 📁 ファイル命名規則

### コンポーネント
- **React Component**: `PascalCase.tsx`
  - 例: `WebGPUCanvas.tsx`, `ControlPanel.tsx`
- **UI Component**: `kebab-case.tsx`
  - 例: `button.tsx`, `card.tsx`

### ユーティリティ
- **Utils**: `camelCase.ts`
  - 例: `mathUtils.ts`, `webgpuUtils.ts`
- **Types**: `types.d.ts`
  - 例: `webgpu-types.d.ts`

### スタイル
- **Global**: `globals.css`
- **Component**: `ComponentName.module.css`（将来使用予定）

## 🔄 ステート管理パターン

### 現在のパターン（Props Drilling）
```typescript
// App.tsx - 中央状態管理
const [webgpuStatus, setWebGpuStatus] = useState<WebGPUStatus>('loading');

// Props経由でのデータ受け渡し
<ControlPanel webgpuStatus={webgpuStatus} />
```

### 将来の拡張（State Manager導入予定）
```typescript
// Zustand使用例
interface AppStore {
  webgpuStatus: WebGPUStatus;
  rendererInfo: RendererInfo;
  setWebGpuStatus: (status: WebGPUStatus) => void;
}

const useAppStore = create<AppStore>((set) => ({
  webgpuStatus: 'loading',
  rendererInfo: null,
  setWebGpuStatus: (status) => set({ webgpuStatus: status })
}));
```

## 🚀 デプロイメント

### 開発環境
```bash
npm run dev     # http://localhost:3000
```

### ステージング環境（準備中）
```bash
npm run build
npm run preview # プロダクションビルドのテスト
```

### プロダクション環境（準備中）
```bash
npm run build
# dist/ フォルダを静的ホスティングサービスにデプロイ
# 推奨: Vercel, Netlify, GitHub Pages
```

## 📈 パフォーマンス監視

### バンドルサイズ分析
```bash
# vite-bundle-analyzer などのツール使用
npm install --save-dev vite-bundle-analyzer
```

### WebGPUパフォーマンス
```typescript
// GPU メモリ使用量監視
const memoryInfo = device.limits;
console.log('Max buffer size:', memoryInfo.maxBufferSize);

// フレームタイム測定
let lastFrameTime = performance.now();
function measureFrameTime() {
  const now = performance.now();
  const deltaTime = now - lastFrameTime;
  lastFrameTime = now;
  return deltaTime;
}
```

## 🔮 今後の拡張予定

### 次期実装タスク
1. **テストフレームワーク導入**
   - Jest + React Testing Library
   - WebGPU モックテスト

2. **開発ツール強化**
   - ESLint + Prettier設定
   - Husky（pre-commitフック）
   - GitHub Actions CI/CD

3. **パフォーマンス最適化**
   - Code splitting
   - Lazy loading
   - Bundle 分析・最適化

4. **開発体験向上**
   - Storybook導入
   - Hot Module Replacement最適化
   - エラーバウンダリ実装

---

このドキュメントは開発進捗に合わせて継続的に更新されます。