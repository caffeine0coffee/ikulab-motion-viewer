# ikulab Motion Viewer - Web Edition

C++/VulkanベースのikulabモーションビューワーのWebGPU + TypeScript移植版

## 🚀 機能

- ✅ WebGPUベースの3Dレンダリング
- ✅ TypeScriptによる型安全性
- ✅ React + Tailwind CSS + shadcn/uiによるモダンUI
- ✅ 基本的な三角形レンダリング（概念実証）
- ✅ リアルタイムレンダリング制御
- ✅ WebGPUデバッグ情報表示
- 🚧 BVHファイル解析（計画中）
- 🚧 3Dシェイプとボーン可視化（計画中）
- 🚧 アニメーション制御（計画中）

## 📋 必要環境

### システム要件
- Node.js 18以上
- WebGPU対応ブラウザ：
  - Chrome 113以上（`chrome://flags/#enable-unsafe-webgpu`を有効化）
  - Edge 113以上（WebGPUフラグを有効化）
  - Firefox Nightly（WebGPUフラグを有効化）

### 技術スタック
- **フロントエンド**: React 18 + TypeScript
- **スタイリング**: Tailwind CSS + shadcn/ui
- **3Dレンダリング**: WebGPU + WGSL
- **ビルドツール**: Vite
- **アイコン**: Lucide React

## 🛠 開発環境セットアップ

### インストール
```bash
cd web
npm install
```

### 開発サーバー起動
```bash
npm run dev
```
アプリケーションは `http://localhost:3000` で利用可能

### プロダクションビルド
```bash
npm run build
```

### プロダクション版プレビュー
```bash
npm run preview
```

## 📁 プロジェクト構造

```
src/
├── components/           # Reactコンポーネント
│   ├── ui/              # shadcn/ui基本コンポーネント
│   │   ├── button.tsx   # ボタンコンポーネント
│   │   ├── card.tsx     # カードコンポーネント
│   │   └── badge.tsx    # バッジコンポーネント
│   ├── WebGPUCanvas.tsx # WebGPUレンダリングキャンバス
│   ├── ControlPanel.tsx # 制御パネル
│   └── StatusOverlay.tsx # ステータス表示
├── lib/                 # ユーティリティ
│   └── utils.ts         # shadcn/ui用ユーティリティ
├── styles/              # スタイルファイル
│   └── globals.css      # Tailwind CSS + カスタムスタイル
├── webgpu-renderer.ts   # WebGPUレンダリングエンジン
├── webgpu-types.d.ts    # WebGPU型定義
├── App.tsx              # メインアプリケーション
└── main.tsx             # アプリケーションエントリーポイント
```

## 🎨 UI/UXの特徴

- **レスポンシブデザイン**: デスクトップ・タブレット・モバイル対応
- **カードベースレイアウト**: 情報の整理された表示
- **リアルタイム状態表示**: WebGPU初期化からレンダリング状態まで
- **アクセシブル**: shadcn/uiによる高いアクセシビリティ
- **ダークモード対応**: CSS変数によるテーマ切り替え準備

## 🔧 WebGPUシェーダー

WGSLシェーダー（WebGPU Shading Language）を使用し、元のC++版のGLSLシェーダーからモダンな代替手段を提供：

- **頂点シェーダー**: 三角形の位置とカラー補間
- **フラグメントシェーダー**: ピクセル色の最終決定
- **リアルタイムレンダリング**: 60FPSターゲット

## 🌐 ブラウザ互換性

WebGPUサポートが必要です。WebGPUが利用できない場合、アプリケーションはエラーメッセージとWebGPU有効化手順を表示します。

## 📈 次期実装予定

### フェーズ1: 基本3D機能
1. キューブレンダリング実装
2. マウスによるカメラ制御（回転・ズーム）
3. 基本的な3Dシェイプライブラリ

### フェーズ2: BVH対応
1. C++からTypeScriptへのBVHパーサー移植
2. ボーン構造の可視化
3. アニメーションタイムライン

### フェーズ3: 高度な機能
1. アニメーション再生制御
2. BVHエクスポート機能
3. パフォーマンス最適化

## 🤝 開発ワークフロー

詳細な開発ガイダンスについては `ARCHITECTURE.md` を参照してください。

## 📄 ライセンス

MIT License - 詳細は `LICENSE.txt` を参照