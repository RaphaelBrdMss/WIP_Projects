//
//  ViewModel.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 10/11/2021.
//

import Foundation
import SwiftUI





struct ImagePick : UIViewControllerRepresentable {
    @Binding var isShown : Bool
    @Binding var imagePicked : Image?
    var sourceType : UIImagePickerController.SourceType = .photoLibrary // TODO cam
    
    func updateUIViewController(_ uiViewController: UIViewControllerType, context: UIViewControllerRepresentableContext<ImagePick>) {
        
    }
    func makeCoordinator() -> PickerCoordinator {
        return PickerCoordinator(isShown: $isShown, imagePicked: $imagePicked)
    }
    
    func makeUIViewController(context: UIViewControllerRepresentableContext<ImagePick>) -> some UIImagePickerController {
        let picker = UIImagePickerController()
        picker.allowsEditing = false
        picker.sourceType = sourceType
       
        picker.delegate = context.coordinator
        return picker
    }
}

class PickerCoordinator : NSObject, UINavigationControllerDelegate, UIImagePickerControllerDelegate{
    
    
    @Binding var isShown : Bool
    @Binding var imagePicked : Image?
    
    init(isShown : Binding<Bool>, imagePicked : Binding<Image?>){
        _isShown = isShown
        _imagePicked = imagePicked
    }
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        var uiImage = info[UIImagePickerController.InfoKey.originalImage] as! UIImage
        uiImage = Wrapper.findCircle(uiImage)
        imagePicked = Image(uiImage: uiImage)
        
        
        
       
        isShown = false
    }
    
    func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        isShown = false
    }
}
