//
//  ContentView.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 08/11/2021.
//

import SwiftUI
import AVFoundation
import CoreMotion

struct ContentView: View {
    @State private var showPicker : Bool = false
    @State private var image : Image? = nil
    
    var body: some View {
        
        
        viewManager()
        
        
    }
}


struct CameraUse : UIViewRepresentable{
    @ObservedObject var manager = MotionManager()
    @ObservedObject var cam : CameraChecker
    func makeUIView(context: Context) ->  UIView {
        
        
        let view = UIView(frame: UIScreen.main.bounds)
        
        
        
        cam.prev = AVCaptureVideoPreviewLayer(session: cam.capture)
        cam.prev.frame = view.frame
        cam.prev.videoGravity = .resizeAspectFill
        view.layer.addSublayer(cam.prev)
        cam.capture.startRunning()
        
        return view
    }
    
    func updateUIView(_ uiView: UIViewType, context: Context) {
        DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
            
            
            if(!cam.isTaken){
                let pitchMin = Float (round(manager.pitch*100)/100)
                let rollMin = Float(round(manager.roll*100)/100)
                
                if(pitchMin == 0.00 && rollMin == 0.00){
                    
                    cam.capturePhoto()
                    
                    
                    
                }
            }
                
            
        }
        
    }
    
}



struct UiviewImage : View{
    @StateObject var cam = CameraChecker()
    @State var retakePhoto: Bool = false
    var body: some View{
        
        if(retakePhoto){
            
            viewManager()
        }
        cam.imagePicked?.resizable()
            .scaledToFit()
        
        HStack{
            Spacer()
            Button("reprendre"){
                cam.retake()
                
                self.retakePhoto = true
                
            }
            .foregroundColor(.black)
            .padding()
            .background(Color.white)
            .cornerRadius(20)
            
            .padding(.trailing, 10)
        }
        
        
        
    }
}

struct cameraView : View{
    @StateObject var camera = CameraChecker()
    
    @ObservedObject var manager = MotionManager()
    
    var body: some View {
        
        
        ZStack{
            
            CameraUse(manager : manager, cam: camera)
            
            
            
            
            Rectangle()
                .strokeBorder(Color.red,lineWidth: 5)
                .frame(width: 75, height: 75, alignment: .center)
                .modifier(ParallaxMotionModifier(manager: manager, magnitude: 100))
                .overlay(
                    Color.blue
                        .frame(width: 65, height: 65, alignment: .center)
                        .modifier(ParallaxMotionModifier(manager: manager, magnitude: 50))
                    ,alignment: .center)
            
            
            
        }
        .onAppear(perform: {
            camera.checkRights()
            
            
        })
        
        
    }
}
///*
struct viewManager: View{
    
    @StateObject var camera = CameraChecker()
    var body: some View{
        
        if(!camera.isTaken){
            //let _ = print("is taken false ")
            cameraView(camera: camera)
        }else{
            //let _ = print("is taken true ")
            UiviewImage(cam: camera)
        }
        
        
    }
}




class CameraChecker : NSObject, AVCapturePhotoCaptureDelegate, ObservableObject  {
    @Published var isTaken = false
    @Published var  capture = AVCaptureSession()
    @Published var alert = false
    @Published var output = AVCapturePhotoOutput()
    @Published var prev : AVCaptureVideoPreviewLayer!
    @Published var picData = Data(count: 0)
    @Published var imageView = UIImage()
    @Published var imagePicked : Image?
    
    
    func checkRights(){
        switch AVCaptureDevice.authorizationStatus(for: .video){
        case .authorized:
            setUpCam()
            return
        case .notDetermined:
            AVCaptureDevice.requestAccess(for: .video){ (status) in
                if status{
                    self.setUpCam()
                }
                
            }
            
            
        case .denied:
            self.alert.toggle()
        case .restricted:
            self.alert.toggle()
        @unknown default:
            return
        }
    }
    
    
    func setUpCam(){
        do{
            self.capture.beginConfiguration()
            let hardwareCam = AVCaptureDevice.default(.builtInWideAngleCamera, for: .video, position: .back)
            
            let input = try AVCaptureDeviceInput(device: hardwareCam!)
            
            if self.capture.canAddInput(input){
                
                self.capture.addInput(input)
            }
            
            
            if self.capture.canAddOutput(self.output){
                
                self.capture.addOutput(self.output)
            }
            
            self.capture.commitConfiguration()
            
        }
        catch{
            print(error.localizedDescription)
        }
    }
    
    func capturePhoto(){
        
        
        
        
            
            self.output.capturePhoto(with: AVCapturePhotoSettings(), delegate: self)
            
            
                
                withAnimation{self.isTaken.toggle()}

        
    }
    
    func retake(){
        
        DispatchQueue.main.async{
            self.picData = Data(count: 0)
            self.imagePicked = nil;
            
            DispatchQueue.main.async{
                withAnimation{self.isTaken.toggle()}
            }
        }
        
        
        
        
    }
    
    func photoOutput(_ output: AVCapturePhotoOutput,
                     didFinishProcessingPhoto photo: AVCapturePhoto,
                     error: Error?){
        if error != nil{
            print("error")
            return
        }
        
        
        
        let photodata = photo.fileDataRepresentation()
        self.picData = photodata!
        imageView = UIImage(data: self.picData)!
        imageView = Wrapper.findCircle(imageView)
        imagePicked = Image(uiImage: imageView)
        
        
        
        print("pic taken")
    }
    
    
}
struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}


