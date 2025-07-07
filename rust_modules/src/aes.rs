#![allow(clippy::missing_safety_doc)]
#[no_mangle]
pub extern "C" fn aes_encrypt(data: *mut u8, len: usize, key: *const [u8; 32]) {
    let key = unsafe { &*key };
    let slice = unsafe { core::slice::from_raw_parts_mut(data, len) };
    for (i, byte) in slice.iter_mut().enumerate() {
        *byte ^= key[i % key.len()];
    }
}

#[no_mangle]
pub extern "C" fn aes_decrypt(data: *mut u8, len: usize, key: *const [u8; 32]) {
    aes_encrypt(data, len, key);
}