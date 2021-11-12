//
//  PhotoCapture.swift
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 10/11/2021.
//

import SwiftUI

struct PhotoCapture: View {
    @Binding var showPicker: Bool
    @Binding var image : Image?
    var body: some View {
        
        ImagePick(isShown: $showPicker, imagePicked: $image)
        
    }
}

struct PhotoCapture_Previews: PreviewProvider {
    static var previews: some View {
        PhotoCapture(showPicker: .constant(false), image: .constant(Image("")))
    }
}
