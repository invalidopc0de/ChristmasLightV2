extern crate simplemad;

use simplemad::{Decoder, Frame};
use std::time::Duration;
use std::fs::File;
use std::path::Path;

fn main() {
    println!("Hello, world!");

    let path = Path::new("audio/WonderfulChristmastime.mp3");
    let file = File::open(&path).unwrap();
    let decoder = Decoder::decode(file).unwrap();

    for decoding_result in decoder {
        match decoding_result {
            Err(e) => println!("Error: {:?}", e),
            Ok(frame) => {
                println!("Frame sample rate: {}", frame.sample_rate);
                println!("First audio sample (left channel): {:?}", frame.samples[0][0]);
                println!("First audio sample (right channel): {:?}", frame.samples[1][0]);
            },
        }
    }
}
