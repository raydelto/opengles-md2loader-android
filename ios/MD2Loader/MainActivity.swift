//
//  MainActivity.swift
//  MD2Loader
//

import UIKit
import GLKit
import OpenGLES

final class MainActivity: UIViewController, GLKViewDelegate, UIGestureRecognizerDelegate {

    private var glSurfaceView: GLKView!
    private var eglContext: EAGLContext!
    private var renderer: MyRenderer?
    private var assetsDirectory: String = ""
    private var scaleFactor: Float = 1.0
    private var displayLink: CADisplayLink?

    private let TAG = "MainActivity"

    override var prefersStatusBarHidden: Bool {
        return true
    }

    override var prefersHomeIndicatorAutoHidden: Bool {
        return true
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        view.backgroundColor = .black

        // Setup OpenGL ES 2.0 context
        guard let context = EAGLContext(api: .openGLES2) else {
            fatalError("Failed to initialize OpenGL ES 2.0 context")
        }
        self.eglContext = context
        EAGLContext.setCurrent(context)

        // Setup GLKView (equivalent to GLSurfaceView)
        glSurfaceView = GLKView(frame: view.bounds, context: context)
        glSurfaceView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        glSurfaceView.contentScaleFactor = UIScreen.main.nativeScale
        glSurfaceView.delegate = self
        glSurfaceView.drawableDepthFormat = .format24
        view.addSubview(glSurfaceView)

        // Setup assets directory in documents folder
        let documentsURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        assetsDirectory = documentsURL.path + "/"

        let assets = [
            "female.md2",
            "female.tga",
            "grunt.md2",
            "grunt.tga",
            "basic.vert",
            "basic.frag"
        ]

        for asset in assets {
            extractAsset(asset)
        }

        // Screen size calculation
        let scale = UIScreen.main.nativeScale
        let width = Int32(view.bounds.width * scale)
        let height = Int32(view.bounds.height * scale)

        renderer = MyRenderer()
        renderer?.setAssetPath(assetsDirectory)
        renderer?.setScreenSize(width: width, height: height)
        renderer?.onSurfaceCreated()

        // Setup Gestures (Scale & Pan)
        setupGestures()

        // Start render loop (equivalent to GLSurfaceView continuous rendering)
        startRenderLoop()
    }

    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        let scale = UIScreen.main.nativeScale
        let width = Int32(view.bounds.width * scale)
        let height = Int32(view.bounds.height * scale)
        renderer?.setScreenSize(width: width, height: height)
    }

    private func setupGestures() {
        let pinchGesture = UIPinchGestureRecognizer(target: self, action: #selector(handlePinch(_:)))
        pinchGesture.delegate = self
        view.addGestureRecognizer(pinchGesture)

        let panGesture = UIPanGestureRecognizer(target: self, action: #selector(handlePan(_:)))
        panGesture.delegate = self
        panGesture.maximumNumberOfTouches = 1
        view.addGestureRecognizer(panGesture)
    }

    @objc private func handlePinch(_ detector: UIPinchGestureRecognizer) {
        if detector.state == .changed {
            scaleFactor *= Float(detector.scale)
            // Don't let the object get too small or too large
            scaleFactor = max(0.1, min(scaleFactor, 5.0))
            print("scalefactor = \(scaleFactor)")
            renderer?.setScale(scaleFactor)
            detector.scale = 1.0
        }
    }

    @objc private func handlePan(_ recognizer: UIPanGestureRecognizer) {
        switch recognizer.state {
        case .began:
            renderer?.setRotationAngles(x: 0.0, y: 0.0)
        case .changed:
            let velocity = recognizer.velocity(in: view)
            // Android's computeCurrentVelocity(2) corresponds to velocity per 2 milliseconds (points/sec * 0.002)
            let xVel = Float(velocity.x) * 0.002
            let yVel = Float(velocity.y) * 0.002
            renderer?.setRotationAngles(x: xVel, y: yVel)
        case .ended, .cancelled:
            renderer?.setRotationAngles(x: 0.0, y: 0.0)
        default:
            break
        }
    }

    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldRecognizeSimultaneouslyWith otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }

    private func extractAsset(_ assetName: String) {
        let destPath = assetsDirectory + assetName
        let fileManager = FileManager.default

        if fileManager.fileExists(atPath: destPath) {
            print("\(assetName) already exists. No extraction needed.")
        } else {
            print("\(assetName) doesn't exist. Extraction needed.")
            let nsName = assetName as NSString
            let base = nsName.deletingPathExtension
            let ext = nsName.pathExtension

            guard let bundlePath = Bundle.main.path(forResource: base, ofType: ext) else {
                print("Failure in extractAsset(): Asset not found in bundle: \(assetName)")
                return
            }

            do {
                try fileManager.copyItem(atPath: bundlePath, toPath: destPath)
                if fileManager.fileExists(atPath: destPath) {
                    print("File \(assetName) extracted successfully")
                }
            } catch {
                print("Failure in extractAssets(): \(error) \n \(assetsDirectory) \(assetName)")
            }
        }
    }

    private func startRenderLoop() {
        displayLink = CADisplayLink(target: self, selector: #selector(renderFrame))
        displayLink?.preferredFrameRateRange = CAFrameRateRange(minimum: 60, maximum: 120, preferred: 60)
        displayLink?.add(to: .main, forMode: .common)
    }

    @objc private func renderFrame() {
        glSurfaceView.display()
    }

    // MARK: - GLKViewDelegate

    private var drawFrameCount = 0

    func glkView(_ view: GLKView, drawIn rect: CGRect) {
        let scale = UIScreen.main.nativeScale
        let width = Int32(rect.width * scale)
        let height = Int32(rect.height * scale)
        if drawFrameCount < 5 {
            NSLog("[MainActivity] glkView drawIn: rect=(%.1f, %.1f, %.1f, %.1f) scale=%.1f w=%d h=%d", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, scale, width, height)
            drawFrameCount += 1
        }
        renderer?.onSurfaceChanged(width: width, height: height)
        renderer?.onDrawFrame()
    }

    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        displayLink?.isPaused = true
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        displayLink?.isPaused = false
    }

    deinit {
        displayLink?.invalidate()
    }
}
