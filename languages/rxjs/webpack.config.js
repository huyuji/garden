module.exports = {
    entry: "./main",
    output: {filename: "app.js"},
    module: {
        rules: [
            {
                test: /.ts$/,
                use: "ts-loader"
            }
        ]
    },
    resolve: {
        extensions: [".ts", ".js"]
    }
}
